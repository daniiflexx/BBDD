#include "../inc/orders.h"

/**
 * @brief execute a query to print the not shipped orders
 *
 *
 * @return EXIT_SUCEEDED if everything is okay or EXIT_FAILURE if there is any error
 * @author Iñigo Alvarez and Daniel Cruz
 */
int PrintOpen() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* ODBC API return status */
    SQLSMALLINT columns = 0; /* number of columns in result-set */
    SQLUSMALLINT ucolumns = 0;
    SQLUSMALLINT i = 0;
    SQLINTEGER x = 0;
    int u = 0;
    #define BufferLength 512
    char y[BufferLength] = "\0";

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }
    /* simple query */
    ret = SQLExecDirect(stmt, (SQLCHAR*) "select ordernumber from orders where shippeddate is NULL order by ordernumber;", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }
    /* How many columns are there */
    ret = SQLNumResultCols(stmt, &columns);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("Error getting number of columns", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* print the name of each column */
    ucolumns = (SQLUSMALLINT)columns;
    for (i = 1; i <= ucolumns; i++) {
        char buf[BufferLength] = "\0";
        ret = SQLDescribeCol(stmt, i, (SQLCHAR *) buf, BufferLength, NULL, NULL, NULL, NULL, NULL);
        if (!SQL_SUCCEEDED(ret)) {
            odbc_extract_error("Error in SQLDescribeCol", stmt, SQL_HANDLE_STMT);
            return ret;
        }
        printf("%s\t", buf);
    }
    printf("\n");

    ret = SQLBindCol(stmt, 1, SQL_C_SLONG, &x, 0, NULL);
    ret = SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR *) y, BufferLength, NULL);
    /* Loop through the rows in the result-set */
    u = 0;
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        u += printf("%d\t%s\n", (int)x, y);
    }
    if (u == 0)
        printf("---No results or invalid input---\n");
    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
 * @brief execute a query to print the required orders between two dates
 *
 *
 * @return EXIT_SUCEEDED if everything is okay or EXIT_FAILURE if there is any error
 * @author Iñigo Alvarez and Daniel Cruz
 */
int PrintRange() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    #define BufferLength 512
    int u= 0;
    char x[BufferLength] = "\0";
    char s[3][BufferLength];
    char y[BufferLength] = "\0";
    char z[BufferLength] = "\0";
    char a[BufferLength] = "\0";



    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR*) "select ordernumber, orderdate, shippeddate from orders where orderdate between ? and ?", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }

    s[0][0]='\0';s[1][0]='\0';s[2][0]='\0';
    printf("Enter dates (YYYY-MM-DD - YYYY-MM-DD) > ");
    (void) fflush(stdout);
    if (fgets(x, (int) sizeof(x), stdin) != NULL) {
        x[strlen(x) - 1] = '\0';
        if (strlen(x) > 22 && strlen(x) < 24) {
            strncpy(s[0], x, 10);
            s[0][10] = '\0';
            strncpy(s[1], x+11, 1);
            s[1][2] = '\0';
            strncpy(s[2], x+13, 10);
            s[2][10] = '\0';
        }
        if (s[0][0] != '\0' && s[1][0] != '\0' && s[2][0] != '\0')
        {
            (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                                    SQL_C_CHAR, SQL_CHAR,
                                    0, 0, s[0], 0, NULL);
            (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT,
                                    SQL_C_CHAR, SQL_CHAR,
                                    0, 0, s[2], 0, NULL);

            (void) SQLExecute(stmt);
            
            (void) SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) y, BufferLength, NULL);
            (void) SQLBindCol(stmt, 2, SQL_C_CHAR,(SQLCHAR *) z, BufferLength, NULL);
            (void) SQLBindCol(stmt, 3, SQL_C_CHAR,(SQLCHAR *) a, BufferLength, NULL);

            /* Loop through the rows in the result-set */
            while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
                u += printf("%s %s %s\n", y, z, a);
            }
        }
        if (u == 0)
                printf("---No results or invalid input---\n");
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
 * @brief execute a query to print an orders and its products
 *
 *
 * @return EXIT_SUCEEDED if everything is okay or EXIT_FAILURE if there is any error
 * @author Iñigo Alvarez and Daniel Cruz
 */
int PrintDetail() {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    #define BufferLength 512
    char x[BufferLength] = "\0";
    char y[BufferLength] = "\0";
    char z[BufferLength] = "\0";
    char a[BufferLength] = "\0";
    char b[BufferLength] = "\0";
    char c[BufferLength] = "\0";
    char d[BufferLength] = "\0";
    int u = 0;




    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret)) {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR*) "with total as (select sum(ordd.priceeach*quantityordered) as importe, ord.orderdate, ord.status from orders ord  natural join orderdetails ordd where ord.ordernumber = ? group by ord.orderdate, ord.status) select total.importe, total.orderdate, total.status, ord.productcode, ord.quantityordered, ord.priceeach from total, orderdetails ord where ord.ordernumber = ? order by ord.orderlinenumber ", SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }


    printf("Enter ordernumber > ");
    (void) fflush(stdout);
    if (fgets(x, (int) sizeof(x), stdin) != NULL) {
        x[strlen(x) - 1] = '\0';
        (void) SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_VARCHAR,
                                0, 0, x, 0, NULL);
        (void) SQLBindParameter(stmt, 2, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_VARCHAR,
                                0, 0, x, 0, NULL);

        (void) SQLExecute(stmt);
        
        (void) SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) y, BufferLength, NULL);
        (void) SQLBindCol(stmt, 2, SQL_C_CHAR,(SQLCHAR *) z, BufferLength, NULL);
        (void) SQLBindCol(stmt, 3, SQL_C_CHAR,(SQLCHAR *) a, BufferLength, NULL);
        (void) SQLBindCol(stmt, 4, SQL_C_CHAR,(SQLCHAR *) b, BufferLength, NULL);
        (void) SQLBindCol(stmt, 5, SQL_C_CHAR,(SQLCHAR *) c, BufferLength, NULL);
        (void) SQLBindCol(stmt, 6, SQL_C_CHAR,(SQLCHAR *) d, BufferLength, NULL);


        
        if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            printf("%s %s\n%s\n%s %s %s\n", z, a, y, b, c, d);
        }
        /* Loop through the rows in the result-set */
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
            u += printf("%s %s %s\n", b, c, d);
        }
        if (u == 0)
            printf("---No results or invalid input---\n");
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

