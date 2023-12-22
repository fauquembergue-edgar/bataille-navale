CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: battleship

battleship: main.c functions.c
	$(CC) $(CFLAGS) -o battleship main.c functions.c

clean:
	rm -f battleship