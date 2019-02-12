## Makefile

CC=gcc -rdynamic -MMD -D_XOPEN_SOURCE=500

CPPFLAGS= `pkg-config --cflags gtk+-3.0`
CFLAGS= -Wall -Wextra -std=c99 -O3 -g
LDFLAGS=
LDLIBS= `pkg-config --libs gtk+-3.0` -lm


SRC= main.c 
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: main

main: ${OBJ}

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main
	rm result/*

-include ${DEP}

# END
