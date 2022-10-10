CC = gcc

CFLAGS=-g -std=gnu11 -Werror -Wall -Wextra -Wpedantic \
			-Wmissing-declarations -Wmissing-prototypes \
			-Wold-style-definition

OBJ = mexec.o

all: mexec

$(OBJ): mexec.c
	$(CC) -c -o $@ $< $(CFLAGS)

mexec: $(OBJ)
	$(CC) -o mexec $^ $(CFLAGS)

