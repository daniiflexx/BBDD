#ifndef ORDERS_H
#define ORDERS_H

#include <stdio.h>
#include <stdlib.h>
#include "odbc.h"

void ShowOrdersMenu();
int PrintOpen();
int PrintDetail();
int PrintRange();
int ShowOrdersSubMenu();
char **ft_split(char const *s, char c);

#endif