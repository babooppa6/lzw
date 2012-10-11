CC=gcc
CFLAGS=-O3 -g3 --std=c99 -Wall

all: encode decode

encode: main.c hasharray.c encode.c decode.c bitio.c stack.c globals.c
	$(CC) $(CFLAGS) -o ../bin/encode $^

decode: encode
	ln -f ../bin/encode ../bin/decode	
