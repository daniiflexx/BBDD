/*
// Created by Daniel and Inigo on 30/8/20.
*/
#include "tester.h"

void checkAddTableEntry(const char * tableName)
/**
 *  check AddIndexEntry
 */
{
    Book bk, bk2, bk3;
    bool result;
    char command[250];
    char *indexName;
    int nodeIDOrDataOffset;
    char prueba[7] = "123456", prueba2[9] = "12345678";

    printf("\n-----TableEntry TESTER------\n");

    indexName = (char *) malloc(sizeof(char) * strlen(tableName) + 1);
    if (!indexName)
        return;
    replaceExtensionByIdx(tableName, indexName);
    sprintf(command, "rm %s %s", tableName, indexName);
    system(command);
    createTable(tableName);
    memcpy(&bk.book_id, "VAR1", 4);
    bk.title = prueba;
    bk.title_len = 6;
    /* 1) add existing node should complain*/
    result = addTableEntry(&bk, tableName, indexName);
    if (result == false){
        free(indexName);
        fprintf(stderr,
                "Error in addTableEntry1\n");
        exit(EXIT_FAILURE);
    }
    /* first add*/
    printf("-----Original tree-------\n");
    printTree(4, indexName);
    memcpy(&bk2.book_id, "VAR2", 4);
    bk2.title = prueba2;
    bk2.title_len = 6;
    result = addTableEntry(&bk2, tableName, indexName);
    if (result == false){
        free(indexName);
        fprintf(stderr,
                "Error in addTableEntry\n");
        exit(EXIT_FAILURE);
    }
    findKey(bk2.book_id, indexName, &nodeIDOrDataOffset);
    printf("-----after adding VAR2-------\n");
    printTree(4, indexName);
    if (nodeIDOrDataOffset!=18){
        free(indexName);
        fprintf(stderr,
                "Error in addTableEntry, inserted offset should be 18"
                " but I get %d", nodeIDOrDataOffset);
        exit(EXIT_FAILURE);
    }
    /*Second add*/
    printf("-----Original tree-------\n");
    printTree(4, indexName);
    memcpy(&bk3.book_id, "VAR3", 4);
    bk3.title = prueba2;
    bk3.title_len = 8;
    result = addTableEntry(&bk3, tableName, indexName);
    if (result == false){
        free(indexName);
        fprintf(stderr,
                "Error in addTableEntry\n");
        exit(EXIT_FAILURE);
    }
    findKey(bk3.book_id, indexName, &nodeIDOrDataOffset);
    printf("-----after adding VAR3-------\n");
    printTree(4, indexName);
    if (nodeIDOrDataOffset!=34){
        free(indexName);
        fprintf(stderr,
                "Error in addTableEntry, inserted offset should be 34"
                " but I get %d", nodeIDOrDataOffset);
        exit(EXIT_FAILURE);
    }
    free(indexName);
    printf("addTableEntry test passed\n");
}


