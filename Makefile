
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic

all: main.o connect.o
	$(CC) $(CFLAGS) main.c connect.c popcl.c args.c -o popcl -lcrypto -lssl

clean:
	rm -f *.o *.out