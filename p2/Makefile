export PGDATABASE:=classicmodels
export PGUSER :=alumnodb
export PGPASSWORD :=alumnodb
export PGCLIENTENCODING:=LATIN9
export PGHOST:=localhost

DBNAME =$(PGDATABASE)
PSQL = psql
CREATEDB = createdb
DROPDB = dropdb --if-exists
PG_DUMP = pg_dump
PG_RESTORE = pg_restore

CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic #-ansi
LDLIBS = -lodbc
# recompile if this heder changes
HEADERS = odbc.h
EXE = menu
SRCS = src/menu.c src/odbc.c src/orders.c src/products.c src/customers.c
OBJS = $(patsubst src/%, obj/%, $(SRCS:.c=.o))
#all : $(EXE)

all : BBDD $(EXE)

compile: $(EXE)

BBDD: dropdb createdb restore

$(EXE): $(OBJS) 
	$(CC) -o $(EXE) $(OBJS) $(LDLIBS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f obj/*.o core $(EXE) 

createdb:
	@echo Creando BBDD
	@$(CREATEDB)
dropdb:
	@echo Eliminando BBDD
	@$(DROPDB) $(DBNAME)
	rm -f *.log
dump:
	@echo creando dumpfile
	@$(PG_DUMP) > $(DBNAME).sql
restore:
	@echo restore data base
	@cat $(DBNAME).sql | $(PSQL)  
psql: shell
shell:
	psql