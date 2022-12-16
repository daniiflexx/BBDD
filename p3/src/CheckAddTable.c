/*
// Created by Daniel on 30/8/20.
*/
#include "tester.h"

void checkAddTableEntry(const char * tableName)
/**
 *  check AddIndexEntry
 */
{
    Book bk, bk2;
    bool result;
    char *indexName;
    int nodeIDOrDataOffset;
    char bookId[4]="   ", prueba[7] = "123456";
    int tmp=-1;
    int deleted;

    if (createTable(tableName) == false) {
        printf("ERROR creating table\n");
        return ;
    }

    indexName = (char *) malloc(sizeof(char) * strlen(tableName) + 1);
    if (!indexName)
        return;
    replaceExtensionByIdx(tableName, indexName);
    memcpy(&bk.book_id, "1234", 4);
    bk.title = prueba;
    bk.title_len = 6;
    /* 1) add existing node should complain*/
    result = addTableEntry(&bk, tableName, indexName);
    if (result == false){
        fprintf(stderr,
                "Error in addTableEntry\n");
        exit(EXIT_FAILURE);
    }
    /* add a new node using a existing deleted node */
    printf("-----Original tree-------\n");
    printTree(4, indexName);
    memcpy(&bk2.book_id, "1235", 4);
    bk2.title = prueba;
    bk2.title_len = 6;
    result = addTableEntry(&bk, tableName, indexName);
    if (result == false){
        fprintf(stderr,
                "Error in addTableEntry\n");
        exit(EXIT_FAILURE);
    }
    findKey(bk2.book_id, indexName, &nodeIDOrDataOffset);
    printf("-----after adding VAR2-------\n");
    printTree(4, indexName);
    if (nodeIDOrDataOffset!=18){
        fprintf(stderr,
                "Error in addTableEntry, inserted offset should be 18"
                " but I get %d", nodeIDOrDataOffset);
        exit(EXIT_FAILURE);
    }
    printf("addTableEntry test passed\n");
}


