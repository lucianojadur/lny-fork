PROGRAM=lyney
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -g
LDFLAGS=-lm


all:$(PROGRAM)

$(PROGRAM): main.o calc.o character.o  
	$(CC) $(CFLAGS) -o $(PROGRAM) main.o calc.o character.o $(LDFLAGS) 

main.o: main.c calc.h character.h lyney.h  
	$(CC) $(CFLAGS) -c main.c

calc.o: calc.c calc.h character.h macros.h lyney.h
	$(CC) $(CFLAGS) -c calc.c

character.o: character.c character.h lyney.h
	$(CC) $(CFLAGS) -c character.c



clean:
	rm -vf  *.o $(PROGRAM)