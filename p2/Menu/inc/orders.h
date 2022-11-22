#ifndef ORDERS_H
#define ORDERS_H

#include <stdio.h>
#include <stdlib.h>
#include "odbc.h"

char **ft_split(char const *s, char c);
int PrintOpen();
int PrintDetail();
int PrintRange();

#endif