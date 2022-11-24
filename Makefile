SRCDIR=src

MAIN-OBJ=${SRCDIR}/compiler.o 
LIB-OBJS=${SRCDIR}/scanner.o #knihovni funkce
REL-OBJS=${MAIN-OBJ} ${LIB-OBJS} #release objects, no tests
SCANNER-TEST-OBJS=${SRCDIR}/scanner-tests.o
SYMTAB-TEST-OBJS=${SRCDIR}/symbtable-tests.o
SYMTAB-TEST-LIB-OBJS=${SRCDIR}/symbtable.o
ALL-OBJS=${REL-OBJS} ${SCANNER-TEST-OBJS} ${SYMTAB-TEST-OBJS} ${SYMTAB-TEST-LIB-OBJS}

CFLAGS=-Wall -Wextra -Wpedantic -g -lm
CC=gcc
PROJECT-NAME=ifj22compiler
SCANNER-TEST-NAME=scanner-test
SYMTABLE-TEST-NAME=symtable-test

all: ${PROJECT-NAME}

${PROJECT-NAME}: ${REL-OBJS}
	${CC} ${CFLAGS} ${REL-OBJS} -o $@

run: ${PROJECT-NAME}
	./${PROJECT-NAME}

symtable-tests: ${SYMTABLE-TEST-NAME}

${SYMTABLE-TEST-NAME}: ${SYMTAB-TEST-OBJS} ${SYMTAB-TEST-LIB-OBJS}
	${CC} ${CFLAGS} ${SYMTAB-TEST-OBJS} ${SYMTAB-TEST-LIB-OBJS} -o $@

scanner-tests: ${SCANNER-TEST-NAME}

${SCANNER-TEST-NAME}: ${LIB-OBJS} ${SCANNER-TEST-OBJS}
	${CC} ${CFLAGS} ${SCANNER-TEST-OBJS} ${LIB-OBJS} -o $@

run-scanner-tests: ${SCANNER-TEST-NAME}
	src/test-skript.sh

run-symtable-tests: $(SYMTABLE-TEST-NAME)
	./symtable-test

clean:
	rm -f ${ALL-OBJS} ${PROJECT-NAME} ${SCANNER-TEST-NAME} ${SYMTABLE-TEST-NAME}
