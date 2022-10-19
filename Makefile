SRCDIR=src
OBJS=${SRCDIR}/compiler.o ${SRCDIR}/scanner.o
CFLAGS=-Wall -Wextra -Wpedantic -g
CC=gcc
PROJECT-NAME=ifj22compiler

${PROJECT-NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@

clean:
	rm -f ${OBJS} ${PROJECT-NAME}
