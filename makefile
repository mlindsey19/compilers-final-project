#Mitch Lindsey
#cs4280
#04-28-2019

CC = gcc
CFLAGS = -g -Wall
OBJ = main.o scanner.o parser.o testTree.o semantics.o

statSem: $(OBJ)
	$(CC)  $(CFLAGS) -o statSem $(OBJ)

%.o: %.c *.h
	$(CC) -c $(CFLAGS) $*.c -o $*.o

clean:
	rm statSem *.o
