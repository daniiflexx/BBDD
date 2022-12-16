CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic -ansi
# LDLIBS = -lodbc

# recompile if this header changes
HEADERS = ./inc/utils.h

EXE = tester
SRCS = src/$(EXE).c src/utils.c src/checkUtils.c src/checkCreateTable.c src/checkReplaceExtensionByIdx.c \
src/checkCreateIndex.c src/checkPrint.o src/checkFindKey.c src/checkAddIndexEntry.c src/checkAddTableEntry.c
OBJS = $(patsubst src/%, obj/%, $(SRCS:.c=.o))
INC = -I inc
all : $(EXE)


obj/%.o: src/%.c $(HEADERS)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c -o $@ $< $(INC)

$(EXE): $(DEPS) $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(LDLIBS) $(INC)

clean :
	rm -f obj/*.o core $(EXE)

fclean: clean
	rm -f tester *.txt *.dat *.idx


