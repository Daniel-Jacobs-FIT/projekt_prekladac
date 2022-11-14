SRCDIR=src

MAIN-OBJ=${SRCDIR}/compiler.o 
LIB-OBJS=${SRCDIR}/scanner.o
REL-OBJS=${MAIN-OBJ} ${LIB-OBJS}
TEST-OBJ=${SRCDIR}/scanner-tests.o
ALL-OBJS=${REL-OBJS} ${TEST-OBJ}

CFLAGS=-Wall -Wextra -Wpedantic -g -lm
CC=gcc
PROJECT-NAME=ifj22compiler
S_TEST_NAME=scanner-test

all: ${PROJECT-NAME}

${PROJECT-NAME}: ${REL-OBJS}
	${CC} ${CFLAGS} ${REL-OBJS} -o $@

run: ${PROJECT-NAME}
	./${PROJECT-NAME}

tests: ${S_TEST_NAME}

${S_TEST_NAME}: ${LIB-OBJS} ${TEST-OBJ}
	${CC} ${CFLAGS} ${TEST-OBJ} ${LIB-OBJS} -o $@

run-tests: ${S_TEST_NAME}
	src/test-skript.sh

clean:
	rm -f ${ALL-OBJS} ${PROJECT-NAME} ${S_TEST_NAME} 
