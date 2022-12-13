/**
 * @file menu.c
 * @author Iñigo Alvarez and Daniel Cruz
 * @date 21 Nobember 2022
 * @brief File containing a menu system with submenus
 *
 * The following program allows a user to select different options
 * from a business menu. The menu has two levels, the top
 * one allows to select between products, orders or customers. The
 * second level (sub-menus) allow to select different options of this areas.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char tableName[140] = "\0";
char indexName[140] = "\0";

void use() {
    char buf[140];

    printf("Table to use: ");
    if (!fgets(buf, 140, stdin)) {
        printf("\nError reading ----->\n");
        return;
    }
    buf[strlen(buf)-1] = 0;
    strcpy(tableName, buf);
    replaceExtensionByIdx(buf, indexName);
    createTable(buf);
}

void insert() {
    char buf[140], buf2[140];
    Book book;

    printf("Key: ");
    if (!fgets(buf2, 4, stdin)) {
        printf("\nError reading ----->\n");
        return;
    }
    memcpy(book.book_id, buf2, 4);
    fflush(stdin);
    printf("Title: ");
    if (!fgets(buf, 140, stdin)) {
        printf("\nError reading ----->\n");
        return;
    }
    buf[strlen(buf)-1] = 0;
    book.title_len = strlen(buf);
    book.title = buf;
    addTableEntry(&book, tableName, indexName);
}

void print() {
    return; 
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
 * @author rmarabini
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
