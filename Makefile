SRCDIR=src

UTILS-OBJS=${SRCDIR}/utils.o

SCANNER-LIB-OBJS=${SRCDIR}/scanner.o
SCANNER-TEST-OBJS=${SRCDIR}/scanner-tests.o

SYMTAB-LIB-OBJS=${SRCDIR}/symbtable.o
SYMTAB-TEST-OBJS=${SRCDIR}/symbtable-tests.o

BOTUP-PARSER-LIB-OBJS=${SRCDIR}/botup-parser.o
BOTUP-PARSER-TEST-OBJS=${SRCDIR}/botup-parser-tests.o

PSA-STACK-LIB-OBJS=${SRCDIR}/psa_stack.o
PSA-STACK-TEST-OBJS=${SRCDIR}/psa_stack-test.o

ALL-OBJS=${SCANNER-TEST-OBJS} ${SCANNER-LIB-OBJS} ${SYMTAB-TEST-OBJS} ${SYMTAB-LIB-OBJS} ${BOTUP-PARSER-LIB-OBJS} ${BOTUP-PARSER-TEST-OBJS} ${PSA-STACK-LIB-OBJS} ${PSA-STACK-TEST-OBJS} ${UTILS-OBJS}

CFLAGS=-Wall -Wextra -Wpedantic -g
CC=gcc
PROJECT-NAME=ifj22compiler
SCANNER-TEST-NAME=scanner-tests
SYMTABLE-TEST-NAME=symtable-tests
BOTUP-PARSER-TEST-NAME=botup-parser-tests
PSA-STACK-TEST-NAME=psa_stack-tests

${SCANNER-TEST-NAME}: ${SCANNER-LIB-OBJS} ${SCANNER-TEST-OBJS} ${UTILS-OBJS}
	${CC} ${CFLAGS} $^ -o $@

run-${SCANNER-TEST-NAME}: ${SCANNER-TEST-NAME}
	src/test-skript.sh scanner

${SYMTABLE-TEST-NAME}: ${SYMTAB-TEST-OBJS} ${SYMTAB-LIB-OBJS} ${UTILS-OBJS}
	${CC} ${CFLAGS} $^ -o $@

run-${SYMTABLE-TEST-NAME}: $(SYMTABLE-TEST-NAME)
	./${SYMTABLE-TEST-NAME}
	valgrind --leak-check=full -s ./${SYMTABLE-TEST-NAME}

${PSA-STACK-TEST-NAME}: ${PSA-STACK-TEST-OBJS} ${PSA-STACK-LIB-OBJS} ${SCANNER-LIB-OBJS} ${UTILS-OBJS}
	${CC} ${CFLAGS} $^ -o $@

run-${PSA-STACK-TEST-NAME}: $(PSA-STACK-TEST-NAME) 
	./${PSA-STACK-TEST-NAME}
	valgrind --leak-check=full -s ./${PSA-STACK-TEST-NAME}

${BOTUP-PARSER-TEST-NAME}: ${SCANNER-LIB-OBJS} ${} ${BOTUP-PARSER-LIB-OBJS} ${BOTUP-PARSER-TEST-OBJS} ${SYMTAB-LIB-OBJS} ${PSA-STACK-LIB-OBJS} ${UTILS-OBJS}
	${CC} ${CFLAGS} $^ -o $@

run-${BOTUP-PARSER-TEST-NAME}: $(BOTUP-PARSER-TEST-NAME)
	src/test-skript.sh parser

clean:
	rm -f ${ALL-OBJS} ${SCANNER-TEST-NAME} ${SYMTABLE-TEST-NAME} ${BOTUP-PARSER-TEST-NAME} ${PSA-STACK-TEST-NAME}
