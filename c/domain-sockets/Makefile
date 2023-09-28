CC := clang

CFLAGS := -Wall

all: server client

server: server.c
	$(CC) $(CFLAGS) server.c -o server.bin

client: client.c
	$(CC) $(CFLAGS) client.c -o client.bin

clean:
	rm -f *.bin
