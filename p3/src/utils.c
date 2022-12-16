/*
 * Created by Iñigo Alvarez and Daniel Cruz
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "utils.h"
#include <sys/stat.h>

static int no_deleted_registers = NO_DELETED_REGISTERS;

/**
 * @brief replace de extension .dat to .idx
 * @author Daniel Cruz and Iñigo Alvarez
 * @param fileName new file name
 * @param indexName file with index
 */
void replaceExtensionByIdx(const char *fileName, char * indexName) {
    size_t l;
    
    if (!fileName || !indexName)
        return;
    l = strlen(fileName);
    indexName[0] = 0;
    strncat(indexName, fileName, l - 3);
    strcat(indexName, "idx");
}

/**
 * @brief create table file if necessary or open it
 * @author Iñigo Alvarez and Daniel Cruz
 * @param tablename file with data
 * @return true or false
 */
bool createTable(const char * tableName) {
    char * indexName;
    int result=EXIT_FAILURE;
    FILE * dataFileHandler = NULL;
    
    if (!tableName)
        return EXIT_FAILURE;
    dataFileHandler = fopen(tableName, "r");
    if (dataFileHandler == NULL) {
        dataFileHandler = fopen(tableName, "wb+");
        if (dataFileHandler == NULL)
        {
            fclose(dataFileHandler);
            return EXIT_FAILURE;
        }
        fwrite(&no_deleted_registers, sizeof(int), 1, dataFileHandler);
    }
    else {
        fclose(dataFileHandler);
        dataFileHandler = fopen(tableName, "rb+");
        if (dataFileHandler == NULL)
            return EXIT_FAILURE;
    }
    fclose(dataFileHandler);
    indexName = (char *) malloc(strlen(tableName) + 1);
    if (!indexName)
        return EXIT_FAILURE;
    replaceExtensionByIdx(tableName, indexName);
    result = createIndex(indexName);
    free(indexName);
    return (result);
}


/**
 * @brief create index file if necessary
 * @author Iñigo Alvarez and Daniel Cruz
 * @param indexname file with index
 * @return true or false
 */
bool createIndex(const char *indexName) {
    FILE * indexFileHandler = NULL;

    if (!indexName)
        return EXIT_FAILURE;
    indexFileHandler = fopen(indexName, "r");
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

/**
 * @brief print nodes
 * @author Iñigo Alvarez and Daniel Cruz
 * @param _level to print nodes
 * @param level to print nodes
 * @param indexFileHandler index file
 * @param node_id
 * @param side
 */
static void printnode(size_t _level, size_t level, FILE * indexFileHandler, int node_id, char side)
 {
    Node node;
    size_t l;
    size_t i;
    size_t INDEX_REGISTER_SIZE;

    if (node_id == -1 || _level>level || !indexFileHandler)
       return;
    INDEX_REGISTER_SIZE = sizeof(node);
    l = INDEX_HEADER_SIZE + INDEX_REGISTER_SIZE * node_id;
    fseek(indexFileHandler, l, SEEK_SET);
    fread(&node, sizeof(Node), 1, indexFileHandler);
    /* _level*3] = '\0';*/
    for (i=0; i< _level; i++)
        printf("   ");
    if (node_id == 0)
        printf("%c %.4s (%d): %d\n", side, node.book_id, node_id, node.offset);
    else
        printf("%c %.4s (%d): %d\n", side, node.book_id, node_id-1, node.offset);

    _level++;
    printnode(_level, level, indexFileHandler, node.left, 'l');
    printnode(_level, level, indexFileHandler, node.right, 'r');
}

/**
 * @brief print the tree
 * @author Iñigo Alvarez and Daniel Cruz
 * @param level: to print the tree
 * @param indexName: file with index
 */
void printTree(size_t level, const char * indexName)
{
    int node_id;
    FILE * indexFileHandler = NULL;

    if (!indexName)
        return;
    indexFileHandler = fopen(indexName, "r");
    if (!indexFileHandler)
        return;
    fread(&node_id, sizeof(int), 1, indexFileHandler);
    if (node_id == -1) {
        printf("Empty file\n");
        fclose(indexFileHandler);
        return ;
    }
    printnode(0, level, indexFileHandler, node_id, ' ');
    fclose(indexFileHandler);
}

/**
 * @author Daniel Cruz and Iñigo Alvarez
 * @brief find the key of book_id
 * @param book_id: key
 * @param indexName: file with index
 * @param nodeIDOrDataOffset: offset or parent node
 * @return true or false
 */
bool findKey(const char * book_id, const char *indexName,int * nodeIDOrDataOffset)
{
    FILE * indexFileHandler = NULL;
    int root=-1;
    Node node;
    size_t l=0, INDEX_REGISTER_SIZE=0;
    int result = 0;

    if (!indexName || !book_id || !nodeIDOrDataOffset)
        return false;
    indexFileHandler = fopen(indexName, "rb+");
    if (!indexFileHandler)
        return false;
    fread(&root, sizeof(int), 1, indexFileHandler);
    INDEX_REGISTER_SIZE = sizeof(Node);
    *nodeIDOrDataOffset = root;

    while(true){
        l = INDEX_HEADER_SIZE + INDEX_REGISTER_SIZE * (*nodeIDOrDataOffset);
        fseek(indexFileHandler, l, SEEK_SET);
        node.left = -1;
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

/**
 * @author Daniel Cruz and Iñigo Alvarez
 * @brief actualizes the index file
 * @param book_id: key
 * @param bookOffset: offset received from addTableEntry
 * @param indexName: name of index file
 * @return true or false
 */
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

    if (!book_id || !indexName)
        return false;
    result = findKey(book_id, indexName, &nodeIDOrDataOffset);
    if (result)
        return false;

    indexFileHandler = fopen(indexName, "rb+");
    if (!indexFileHandler) {
        printf("Error reading .idx file\n");
        return false;
    }
    result = fread(&first, sizeof(int), 1, indexFileHandler);
    result = fread(&deleted, sizeof(int), 1, indexFileHandler);
    if (deleted != -1) {
        result = fseek(indexFileHandler,
              deleted * INDEX_REGISTER_SIZE + INDEX_HEADER_SIZE,
              SEEK_SET);
        result = fread(&node, INDEX_REGISTER_SIZE, 1, indexFileHandler);
        new_node = deleted;
        deleted = node.left;
        result = fseek(indexFileHandler, sizeof(int), SEEK_SET);
        result = fwrite(&deleted, sizeof(int), 1, indexFileHandler);
        result = fseek(indexFileHandler,
              new_node * INDEX_REGISTER_SIZE + INDEX_HEADER_SIZE,
              SEEK_SET);
    }
    else{
        result = fseek(indexFileHandler, 0, SEEK_END);
        stat(indexName, &st);
        new_node = ((st.st_size - INDEX_HEADER_SIZE) / INDEX_REGISTER_SIZE );
        if (first == -1) {
            fseek(indexFileHandler, sizeof(int), SEEK_SET);
            fclose(indexFileHandler);
            indexFileHandler = fopen(indexName, "w");
            fclose(indexFileHandler);
            indexFileHandler = fopen(indexName, "rb+");
            fwrite(&new_node, sizeof(int), 1, indexFileHandler);
            fwrite(&menos_uno, sizeof(int), 1, indexFileHandler);
            fprintf(stderr,"new_node=%d", new_node);
        }
        else
            fprintf(stderr,"new_node=%d", new_node - 1);

    }
    memcpy(node.book_id, book_id,4);
    node.offset = bookOffset;
    node.right = -1;
    node.left = -1;
    node.parent = nodeIDOrDataOffset;
    result = fwrite(&node, INDEX_REGISTER_SIZE, 1, indexFileHandler);
    result = fseek(indexFileHandler, nodeIDOrDataOffset * INDEX_REGISTER_SIZE + INDEX_HEADER_SIZE,        SEEK_SET);
    result = fread(&node, INDEX_REGISTER_SIZE, 1, indexFileHandler);
    if (memcmp(book_id,node.book_id,4) <0)
        node.left = new_node;
    else
        node.right = new_node;
    result = fseek(indexFileHandler, nodeIDOrDataOffset * INDEX_REGISTER_SIZE + INDEX_HEADER_SIZE,    SEEK_SET);
    result = fwrite(&node, INDEX_REGISTER_SIZE, 1, indexFileHandler);
    result = fclose(indexFileHandler);
    return true;
}

#ifndef DENERDEFINED
#define DENERDEFINED
/**
 * @author Daniel Cruz and Iñigo Alvarez
 * @brief add information and calls addIndexEntry
 * @param book
 * @param dataName: name of the data file
 * @param indexName: name of index file
 * @return true or false
 */
bool addTableEntry(Book * book, const char * dataName,
                   const char * indexName)
{
    int lenTitle = 0;
    int nodeIDOrDataOffset = 0;
    int result = -1;
    FILE *dataFileHandler = NULL;
    int deleted = -1;
    int offset = 0;
    struct stat st;

    if (!book || !dataName || !indexName)
        return false;

    result = findKey(book->book_id, indexName, &nodeIDOrDataOffset);
    if (result)
    {
        fprintf(stderr, "\nThese key has been already introduced. Try another key ----->\n");
        return false;
    }
    dataFileHandler = fopen(dataName, "rb+");
    if (!dataFileHandler) {
        printf("Error reading .dat file\n");
        return false;
    }
    fread(&deleted, sizeof(int), 1, dataFileHandler);
    if (deleted == -1) {
        lenTitle = strlen(book->title);
        stat(dataName, &st);
        offset = st.st_size;
        fseek(dataFileHandler, 0, SEEK_END);
        fwrite(book->book_id, PK_SIZE, 1, dataFileHandler);
        fwrite(&lenTitle, sizeof(int), 1, dataFileHandler);
        fwrite(book->title, lenTitle, 1, dataFileHandler);
    }
    else
        printf("Queda fuera de nuestro control en esta práctica.\n\n");
    if (addIndexEntry(book->book_id, offset, indexName) == false)
    {
        fclose(dataFileHandler);
        return false;
    }
    
    fclose(dataFileHandler);
    return true;
}
#endif