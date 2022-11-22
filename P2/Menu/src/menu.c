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
#include "../inc/menu.h"

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
        printf("This is a menu of classicmodels\n");
        printf("You can choose to display a number of different options\n\n");

        printf(" (1) Products\n"
               " (2) Orders\n"
               " (3) Customers\n"
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
 * @brief Shows and processes the products submenu.
 *
 * This function implements a loop that prints the products
 * menu and returns the selected option
 * @return int with the selected option
 * @author Iñigo Alvarez and Daniel Cruz
 */
static int ShowProductsSubMenu() {
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

/**
 * @brief Shows and processes the products menu.
 *
 * This function implements a loop analyze the user selection and prints
 * the selected option
 * @return void
 * @author Iñigo Alvarez and Daniel Cruz
 */
static void ShowProductsMenu() {
    int nChoice = 0;
    do {
        nChoice = ShowProductsSubMenu();
        switch (nChoice) {

            case 1: {
                if (PrintStock() == EXIT_FAILURE)
                    return;
            }
                break;

            case 2: {
                if (PrintFind() == EXIT_FAILURE)
                    return;
            }
                break;

            case 3: {
                printf("Back\n\n");
            }
                break;
        }
    } while (nChoice != 3);
}

/**
 * @brief Shows and processes the orders submenu.
 *
 * This function implements a loop that prints the orders
 * menu and returns the selected option
 * @return int with the selected option
 * @author Iñigo Alvarez and Daniel Cruz
 */
static int ShowOrdersSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Print open\n"
               " (2) Print range\n"
               " (3) Print detail\n"
               " (4) Print back\n\n");

        printf("Enter a number that corresponds to your choice > ");
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
 * @brief Shows and processes the orders menu.
 *
 * This function implements a loop analyze the user selection and prints
 * the selected option
 * @return void
 * @author Iñigo Alvarez and Daniel Cruz
 */
static void ShowOrdersMenu() {
    int nChoice = 0;
    do {
        nChoice = ShowOrdersSubMenu();
        switch (nChoice) {

            case 1: {
                if (PrintOpen() == EXIT_FAILURE)
                    return;
            }
                break;

            case 2: {
                if (PrintRange() == EXIT_FAILURE)
                    return;
            }
                break;

            case 3: {
                if (PrintDetail() == EXIT_FAILURE)
                    return;
            }
                break;
            case 4: {
                printf("Back\n\n");
            }
                break;
        }
    } while (nChoice != 4);
}

/**
 * @brief Shows and processes the customers submenu.
 *
 * This function implements a loop that prints the customers
 * menu and returns the selected option
 * @return int with the selected option
 * @author Iñigo Alvarez and Daniel Cruz
 */
static int ShowCustomersSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Find\n"
               " (2) List Products\n"
               " (3) Balance\n"
               " (4) Back\n\n");

        printf("Enter a number that corresponds to your choice > ");
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
 * @brief Shows and processes the customers menu.
 *
 * This function implements a loop analyze the user selection and prints
 * the selected option
 * @return void
 * @author Iñigo Alvarez and Daniel Cruz
 */
static void ShowCustomersMenu() {
    int nChoice = 0;
    do {
        nChoice = ShowCustomersSubMenu();
        switch (nChoice) {

            case 1: {
                if (Find() == EXIT_FAILURE)
                    return;
            }
                break;

            case 2: {
                if (ListProducts() == EXIT_FAILURE)
                    return;
            }
                break;

            case 3: {
                if (Balance() == EXIT_FAILURE)
                    return;
            }
                break;

            case 4: {
                printf("Back\n\n");
            }
                break;
        }
    } while (nChoice != 4);
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
                ShowProductsMenu();
            }
                break;

            case 2: {
                ShowOrdersMenu();
            }
                break;
            
            case 3: {
                ShowCustomersMenu();
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