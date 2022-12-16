/**
 * @file menu.c
 * @author Iñigo Alvarez and Daniel Cruz
 * @date 21 Nobember 2022
 * @brief File containing a menu system with submenus
 *
 * The following program allows a user to select different options
 * from a database menu.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char tableName[140] = "\0";
char indexName[140] = "\0";

/**
 * @author Daniel Cruz and Iñigo Alvarez
 * @brief starts a table
 */
void use() {
    char buf[140];

    printf("Table to use: ");
    fflush(stdout);
    if (!fgets(buf, 140, stdin)) {
        printf("\nError reading ----->\n");
        return;
    }
    fflush(stdout);
    buf[strlen(buf)-1] = 0;
    if (strlen(buf) < 5 || strncmp(buf + strlen(buf) - 4, ".dat", 4) != 0) {
        printf("ERROR, file must be a valid .dat\n\n");
        return ;
    }
    strcpy(tableName, buf);
    replaceExtensionByIdx(buf, indexName);
    createTable(buf);
}

/**
 * @author Daniel Cruz and Iñigo Alvarez
 * @brief inserts a tuple
 */
void insert() {
    char buf[140] = "\0", buf2[140] = "\0";
    Book book;

    if (tableName[0] == '\0' || indexName[0] == '\0' ) {
        printf("ERROR, use() function needs to be used first\n\n");
        return ;
    }

    printf("Key: ");
    fflush(stdout);
    if (!fgets(buf2, 140, stdin)) {
        printf("\nError reading ----->\n");
        return;
    }
    if (strlen(buf2) != 5)
    {
        printf("\nKey must be size 4 ----->\n");
        return;
    }
    buf2[4] = '\0';
    memcpy(book.book_id, buf2, 4);
    printf("Title: ");
    fflush(stdout);
    if (!fgets(buf, (int)sizeof(buf), stdin)) {
        printf("\nError reading ----->\n");
        return;
    }
    buf[strlen(buf)-1] = 0;
    book.title_len = strlen(buf);
    book.title = buf;
    if (addTableEntry(&book, tableName, indexName) == false) {
        printf("ERROR inserting tuple\n\n");
    }
}

/**
 * @author Daniel Cruz and Iñigo Alvarez
 * @brief prints the table
 * 
 */
void print() {
    
    if (tableName[0] == '\0' || indexName[0] == '\0' ) {
        printf("ERROR, use() function needs to be used first\n\n");
        return ;
    }
    
    printTree(4, indexName);
}
/**
 * @brief prints main menu and allows to select an option.
 *
 *
 * @return selected entry in the menu
 * @author Iñigo Alvarez and Daniel Cruz
 */

static int ShowMainMenu() {
    int nSelected = 0;
    char buf[16]; /* buffer for reading user input
                     in theory we only need 1 char
                     but a large buffer will handle
                     more gracefully wrong inputs
                     as 'qwerty' */

    do {
        printf("This is a menu of my database\n");
        printf("You can choose to display a number of different options\n\n");

        printf(" (1) Use\n"
               " (2) Insert\n"
               " (3) Print\n"
               " (4) Exit\n\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

/**
 * @brief Entry point, shows and process main menu.
 *
 * This function implements a loop that prints the main
 * menu, analyze the user selection and calls the
 * corresponding submenu
 *
 * @return 0 if no error
 * @author Iñigo Alvarez and Daniel Cruz
 */
int main(void) {
    int nChoice = 0;

    do {
        nChoice = ShowMainMenu();
        switch (nChoice) {
            case 1: {
                use();
            }
                break;

            case 2: {
                insert();
            }
                break;
            
            case 3: {
                print();
            }
                break;
            
            case 4: {
                printf("Bye Bye\n\n");
            }
                break;
        }
    } while (nChoice != 4);
    return 0;
}
