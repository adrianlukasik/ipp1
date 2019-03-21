CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS =

.PHONY: all clean

all: quantization

quantization: main.o list.o tree.o parse.o
	$(CC) $(LDFLAGS) -o $@ $^

list.o: list.c list.h
	$(CC) $(CFLAGS) -c $<

tree.o: tree.c tree.h list.h
	$(CC) $(CFLAGS) -c $<
	
parse.o: parse.c parse.h tree.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c parse.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o quantization
