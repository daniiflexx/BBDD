/**
 * @file customers.c
 * @author Iñigo Alvarez and Daniel Cruz
 * @date 21 Nobember 2022
 * @brief File containing the customer functions
 *
 */

#include "../inc/customers.h"


/**
 * @brief execute a query to find a user name
 *
 *
 * @return EXIT_SUCEEDED if everything is okay or EXIT_FAILURE if there is any error
 * @author Iñigo Alvarez and Daniel Cruz
 */
int Find() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    #define BufferLength 512
    char x[BufferLength] = "\0";
    char y[BufferLength] = "\0";
    char k[BufferLength] = "\0";
    char j[BufferLength] = "\0";
    char h[BufferLength] = "\0";
    char aux[BufferLength] = "\0";
    int i = 0;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR*) "select customernumber, customername, contactfirstname, contactlastname FROM customers WHERE contactfirstname LIKE ? or contactlastname LIKE ? order by customernumber;", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }


    printf("Enter customer name > ");
    (void) fflush(stdout);
    
    if (fgets(x, (int) sizeof(x), stdin) != NULL) {
        x[strlen(x)-1] = '\0';
        aux[0] = '%';
        while (x[i] != '\0')
        {
            aux[i+1] = x[i];
            i++;
        }
        aux[i+1] = '%';
        
        (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_VARCHAR,
                                0, 0, aux, 0, NULL);
        (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_VARCHAR,
                                0, 0, aux, 0, NULL);

        (void) SQLExecute(stmt);
        
        (void) SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) k, BufferLength, NULL);
        (void) SQLBindCol(stmt, 2, SQL_C_CHAR,(SQLCHAR *) y, BufferLength, NULL);
        (void) SQLBindCol(stmt, 3, SQL_C_CHAR,(SQLCHAR *) j, BufferLength, NULL);
        (void) SQLBindCol(stmt, 4, SQL_C_CHAR,(SQLCHAR *) h, BufferLength, NULL);

        i = 0;
        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("%s %s %s %s\n", k, y, j, h);
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

/**
 * @brief execute a query that list some products
 *
 *
 * @return EXIT_SUCEEDED if everything is okay or EXIT_FAILURE if there is any error
 * @author Iñigo Alvarez and Daniel Cruz
 */
int ListProducts() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    #define BufferLength 512
    char x[BufferLength] = "\0";
    char y[BufferLength] = "\0";
    char k[BufferLength] = "\0";

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR*) "select p.productname, Sum(ord.quantityordered) FROM products p JOIN orderdetails ord ON p.productcode = ord.productcode JOIN orders o ON ord.ordernumber = o.ordernumber JOIN customers c ON o.customernumber = c.customernumber WHERE c.customernumber = ? GROUP BY p.productname, p.productcode ORDER BY p.productcode;", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }


    printf("Enter customer number > ");
    (void) fflush(stdout);
    
    if (fgets(x, (int) sizeof(x), stdin) != NULL) {
        x[strlen(x)-1] = '\0';        
        (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_VARCHAR,
                                0, 0, x, 0, NULL);

        (void) SQLExecute(stmt);
        
        (void) SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) k, BufferLength, NULL);
        (void) SQLBindCol(stmt, 2, SQL_C_CHAR,(SQLCHAR *) y, BufferLength, NULL);

        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("%s %s\n", k, y);
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

/**
 * @brief execute a query to find the balance of a user
 *
 *
 * @return EXIT_SUCEEDED if everything is okay or EXIT_FAILURE if there is any error
 * @author Iñigo Alvarez and Daniel Cruz
 */
int Balance() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    #define BufferLength 512
    char x[BufferLength] = "\0";
    char k[BufferLength] = "\0";

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR*) "select (select Sum(pay.amount) FROM payments pay JOIN customers c ON pay.customernumber = c.customernumber WHERE c.customernumber = ?) - (select Sum(ord.priceeach * ord.quantityordered) FROM orderdetails ord JOIN orders o ON ord.ordernumber = o.ordernumber JOIN customers c ON o.customernumber = c.customernumber WHERE c.customernumber = ?);", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }


    printf("Enter customer number > ");
    (void) fflush(stdout);
    
    if (fgets(x, (int) sizeof(x), stdin) != NULL) {
        x[strlen(x)-1] = '\0';        
        (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_VARCHAR,
                                0, 0, x, 0, NULL);

        (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_VARCHAR,
                                0, 0, x, 0, NULL);

        (void) SQLExecute(stmt);
        
        (void) SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) k, BufferLength, NULL);

        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("Balance = %s\n", k);
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