CC = gcc
CFLAGS = -Wall -lrt -pthread

all: server client

server: server.c
	$(CC) server.c -o server $(CFLAGS)
client: client.c
	$(CC) client.c -o client $(CFLAGS)

clean:
	rm -rf server client *.o
