CC = gcc
CFLAGS = -Wall -std=c99

.PHONY: test clean

program: main.o 
	$(CC) $(CFLAGS) -o program main.o

limitBreak: limitBreak.o
	$(CC) $(CFLAGS) -o limitBreak limitBreak.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

test: main
	./main in.txt

clean: 
	rm -f *.o program limitBreak
