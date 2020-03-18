# Makefile for NN (Neural-Network)

CC = gcc -fsanitize=address
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -std=c99 -O3 -g
LDFLAGS =
LDLIBS =

SRC= main.c matrix.c sigmoid.c
OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: main

main: ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} main

-include ${DEP}

# END
