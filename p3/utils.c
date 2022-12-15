/*
 * Created by roberto on 27/8/20.
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "utils.h"
#include <sys/stat.h>

static int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char * indexName) {
    size_t l;
    l = strlen(fileName);
    indexName[0] = 0;
    strncat(indexName, fileName, l - 3);
    strcat(indexName, "idx");
}


bool createTable(const char * tableName) {
    char * indexName;
    int result=EXIT_FAILURE;
    FILE * dataFileHandler = NULL;
    /* try to open the file */
    dataFileHandler = fopen(tableName, "r");
    /* if file does not exist, create and init it */
    if (dataFileHandler == NULL) {
        dataFileHandler = fopen(tableName, "wb+");
        if (dataFileHandler == NULL)
            return EXIT_FAILURE;
        fwrite(&no_deleted_registers, sizeof(int), 1, dataFileHandler);
    }
    else {
        fclose(dataFileHandler);
        dataFileHandler = fopen(tableName, "rb+");
    }
    fclose(dataFileHandler);
    /* call createIndex */
    /* alloc memory for output string */
    indexName = (char *) malloc(strlen(tableName) + 1);
    replaceExtensionByIdx(tableName, indexName);
    result = createIndex(indexName);
    free(indexName);
    return (result);
}

bool createIndex(const char *indexName) {
    FILE * indexFileHandler = NULL;
    /* try to open the file */
    indexFileHandler = fopen(indexName, "r");
    /* if file does not exist, create and init it */
    if (indexFileHandler == NULL) {
        indexFileHandler = fopen(indexName, "wb+");
        if (indexFileHandler == NULL)
            return EXIT_FAILURE;
        fwrite(&no_deleted_registers, sizeof(int), 1, indexFileHandler);
        fwrite(&no_deleted_registers, sizeof(int), 1, indexFileHandler);
    }
    else {
        fclose(indexFileHandler);
        indexFileHandler = fopen(indexName, "rb+");
    }
    fclose(indexFileHandler);
    return EXIT_SUCCESS;
}
static void printnode(size_t _level, size_t level, FILE * indexFileHandler, int node_id, char side)
/**
 *
 * @param _level
 * @param level
 * @param indexFileHandler
 * @param node_id
 * @param side
 */
 {
    Node node;
    size_t l;
    size_t i;
    size_t INDEX_REGISTER_SIZE;

    if (node_id == -1)
       return;
    if (_level>level)
        return;
    INDEX_REGISTER_SIZE = sizeof(node);
    l = INDEX_HEADER_SIZE + INDEX_REGISTER_SIZE * node_id;
    fseek(indexFileHandler, l, SEEK_SET);
    fread(&node, sizeof(Node), 1, indexFileHandler);
    /* _level*3] = '\0';*/
    for (i=0; i< _level; i++)
        printf("   ");
    printf("%c %.4s (%d): %d\n",
           side, node.book_id, node_id, node.offset);

    _level++;
    printnode(_level, level, indexFileHandler, node.left, 'l');
    printnode(_level, level, indexFileHandler, node.right, 'r');
}


void printTree(size_t level, const char * indexName)
/**
 *
 * @param level: print the index tree up to this level
 * @param indexName: file with index
 */
{
    int node_id;
    FILE * indexFileHandler = NULL;

    /* try to open the file */
    indexFileHandler = fopen(indexName, "r");
    /* where is root? */
    fread(&node_id, sizeof(int), 1, indexFileHandler);
    if (node_id == -1) {
        printf("error\n");
        return ;
    }
    /* read first node */
    printnode(0, level, indexFileHandler, node_id, ' ');
}

bool findKey(const char * book_id, const char *indexName,
             int * nodeIDOrDataOffset)/**
 * find key book_id
 * @param book_id: key to look for
 * @param indexName: file with index
 * @param nodeIDOrDataOffset: offset if retirn=true,
 *             otherwise parent node to ass new key
 * @return true if true if offset found
 */
 {
    FILE * indexFileHandler = NULL;
    int root=-1;
    Node node;
    size_t l=0, INDEX_REGISTER_SIZE=0;
    int result;
    /* fetch root node */
    indexFileHandler = fopen(indexName, "rb+");
    fread(&root, sizeof(int), 1, indexFileHandler);
    INDEX_REGISTER_SIZE = sizeof(Node);
    *nodeIDOrDataOffset = root;

    while(true){
        l = INDEX_HEADER_SIZE + INDEX_REGISTER_SIZE * (*nodeIDOrDataOffset);
        fseek(indexFileHandler, l, SEEK_SET);
        fread(&node, sizeof(Node), 1, indexFileHandler);
        result = memcmp(book_id, node.book_id,PK_SIZE);
        if (result == 0){
            *nodeIDOrDataOffset = node.offset;
            fclose(indexFileHandler);
            return true;
        }
        else if (result < 0) {
            if (node.left  == -1 || root == -1) {
                fclose(indexFileHandler);
                return false;
            } else
                *nodeIDOrDataOffset = node.left;
        }
        else {
            if (node.right == -1 || root == -1) {
                fclose(indexFileHandler);
                return false;
            } else
                *nodeIDOrDataOffset = node.right;
        }
    }
}

bool addIndexEntry(char * book_id,  int bookOffset, char const * indexName) {
    int nodeIDOrDataOffset;
    int result = -1;
    FILE *indexFileHandler = NULL;
    int deleted = -1, first = -1;
    int new_node = -1;
    int menos_uno = -1;
    size_t INDEX_REGISTER_SIZE = sizeof(Node);
    Node node;
    struct stat st;

    /* if key exists returns false */
    result = findKey(book_id, indexName, &nodeIDOrDataOffset);
    if (result)
        return false;

    /* add new key */
    /* check for deleted registers */
    indexFileHandler = fopen(indexName, "rb+");
    /* skip pointer to root node */
    result = fread(&first, sizeof(int), 1, indexFileHandler);
    /* read pointer to deleted list of registers */
    result = fread(&deleted, sizeof(int), 1, indexFileHandler);
    /* use available space for new node if possible*/
    if (deleted != -1) {
        /* read deleted node */
        result = fseek(indexFileHandler,
              deleted * INDEX_REGISTER_SIZE + INDEX_HEADER_SIZE,
              SEEK_SET);
        result = fread(&node, INDEX_REGISTER_SIZE, 1, indexFileHandler);
        new_node = deleted;
        deleted = node.left;
        /* update header node */
        result = fseek(indexFileHandler, sizeof(int), SEEK_SET);
        result = fwrite(&deleted, sizeof(int), 1, indexFileHandler);
        /* move pointer to node to be recicled*/
        result = fseek(indexFileHandler,
              new_node * INDEX_REGISTER_SIZE + INDEX_HEADER_SIZE,
              SEEK_SET);
    }
    /* add node at the end of the file */
    else{
        result = fseek(indexFileHandler, 0, SEEK_END);
        stat(indexName, &st);
        new_node = ((st.st_size - INDEX_HEADER_SIZE) / INDEX_REGISTER_SIZE );
        fseek(indexFileHandler, sizeof(int), SEEK_SET);
        if (first == -1) {
            fclose(indexFileHandler);
            indexFileHandler = fopen(indexName, "w");
            fclose(indexFileHandler);
            indexFileHandler = fopen(indexName, "rb+");
            fwrite(&new_node, sizeof(int), 1, indexFileHandler);
            fwrite(&menos_uno, sizeof(int), 1, indexFileHandler);
        }
        fprintf(stderr,"new_node=%d", new_node);
    }
    memcpy(node.book_id, book_id,4);
    node.offset = bookOffset;
    node.right = -1;
    node.left = -1;
    node.parent = nodeIDOrDataOffset;
    result = fseek(indexFileHandler, 0, SEEK_END);
    result = fwrite(&node, INDEX_REGISTER_SIZE, 1, indexFileHandler);
    /* read parent node and update it */
    result = fseek(indexFileHandler,
    nodeIDOrDataOffset * INDEX_REGISTER_SIZE + INDEX_HEADER_SIZE,        SEEK_SET);
    result = fread(&node, INDEX_REGISTER_SIZE, 1, indexFileHandler);
    if (memcmp(book_id,node.book_id,4) <0)
        node.left = new_node;
    else
        node.right = new_node;
    result = fseek(indexFileHandler,
    nodeIDOrDataOffset * INDEX_REGISTER_SIZE + INDEX_HEADER_SIZE,    SEEK_SET);
    result = fwrite(&node, INDEX_REGISTER_SIZE, 1, indexFileHandler);
    result = fclose(indexFileHandler);
    return true;
}

#ifndef DENERDEFINED
#define DENERDEFINED
bool addTableEntry(Book * book, const char * dataName,
                   const char * indexName)
{
    int lenTitle = 0;
    int nodeIDOrDataOffset;
    int result = -1;
    FILE *dataFileHandler = NULL;
    int deleted = -1;
    int offset;
    struct stat st;

    /* if key exists returns false */
    result = findKey(book->book_id, indexName, &nodeIDOrDataOffset);
    if (result)
        return false;

    /* add new key */
    /* check for deleted registers */
    dataFileHandler = fopen(dataName, "rb+");
    /* read pointer to deleted list of registers */
    fread(&deleted, sizeof(int), 1, dataFileHandler);
    /* use available space for new node if possible*/
    if (deleted == -1) {
        lenTitle = strlen(book->title);
        stat(indexName, &st);
        offset = st.st_size;
        fseek(dataFileHandler, 0, SEEK_END);
        fwrite(book->book_id, PK_SIZE, 1, dataFileHandler);
        fwrite(&lenTitle, sizeof(int), 1, dataFileHandler);
        fwrite(book->title, lenTitle, 1, dataFileHandler);
    }
    /* add node at the end of the file */
    else
        printf("Queda fuera de nuestro control. CALLAO!!!");
    addIndexEntry(book->book_id, offset, indexName);
    
    fclose(dataFileHandler);
    return true;
}
#endif