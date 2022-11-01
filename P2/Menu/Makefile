CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic #-ansi
LDLIBS = -lodbc
# recompile if this heder changes
HEADERS = odbc.h
EXE = menu
SRCS = mimenu.c odbc.c
OBJS = $(patsubst %, %, $(SRCS:.c=.o))
#all : $(EXE)

all : $(EXE)

$(EXE): $(OBJS) 
	$(CC) -o $(EXE) $(OBJS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

clean :
	rm -f *.o core $(EXE) 
