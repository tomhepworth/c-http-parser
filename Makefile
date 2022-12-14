CC=gcc
CFLAGS=-g

%.o: %.c http-parser.h
	$(CC) -c -o $@ $< $(CFLAGS)

demo:	http-parser.o
	$(CC) demo.c -o demo http-parser.o

