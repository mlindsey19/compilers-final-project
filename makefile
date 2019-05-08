#Mitch Lindsey
#cs4280
#04-28-2019

CC = gcc
CFLAGS = -g -Wall
OBJ = main.o scanner.o parser.o testTree.o semantics.o

comp: $(OBJ)
	$(CC)  $(CFLAGS) -o comp $(OBJ)

%.o: %.c *.h
	$(CC) -c $(CFLAGS) $*.c -o $*.o

clean:
	rm comp *.o *.asm
