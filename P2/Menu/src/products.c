#include "products.h"
#include <string.h>

/**
 * @brief Shows and processes the rhyme menu.
 *
 * This function implements a loop that prints the rhyme
 * menu, analyze the user selection and prints
 * the selected rhyme
 * @return void
 * @author rmarabini
 */

void ShowProductsMenu() {
    int nChoice = 0;
    do {
        nChoice = ShowProductsSubMenu();
        switch (nChoice) {

            case 1: {
                PrintStock();
            }
                break;

            case 2: {
                PrintFind();
            }
                break;

            case 3: {
                printf("Back\n\n");
            }
                break;
        }
    } while (nChoice != 3);

}

 int ShowProductsSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Print stock\n"
               " (2) Print find\n"
               " (3) Print back\n\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");


        if ((nSelected < 1) || (nSelected > 3)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 3));
    
    return nSelected;
}

int PrintStock()
{
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    #define BufferLength 512
    char x[BufferLength] = "\0";
    char y[BufferLength] = "\0";

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR*) "select quantityinstock from products where productcode = ?", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }


    printf("Enter productcode > ");
    (void) fflush(stdout);
    if (fgets(x, (int) sizeof(x), stdin) != NULL) {
        x[strlen(x) - 1] = 0;
        (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_VARCHAR,
                                0, 0, x, 0, NULL);

        (void) SQLExecute(stmt);
        
        (void) SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) y, BufferLength, NULL);

        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("Quantity in stock = %s\n", y);
        }
        printf("\n");
        (void) SQLCloseCursor(stmt);

        (void) fflush(stdout);
    }

    /* free up statement handle */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void PrintFind() {
    printf("hola");
}