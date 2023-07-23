PROGRAM=lyney
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -g
LDFLAGS=-lm


all:$(PROGRAM)

$(PROGRAM): main.o calc.o character.o output.o
	$(CC) $(CFLAGS) -o $(PROGRAM) main.o calc.o character.o output.o $(LDFLAGS) 

main.o: src/main.c src/calc.h src/character.h src/lyney.h src/output.h
	$(CC) $(CFLAGS) -c src/main.c

calc.o: src/calc.c src/calc.h src/character.h src/macros.h src/lyney.h
	$(CC) $(CFLAGS) -c src/calc.c

character.o: src/character.c src/character.h src/lyney.h
	$(CC) $(CFLAGS) -c src/character.c

output.o: src/output.c src/output.h 
	$(CC) $(CFLAGS) -c src/output.c

clean:
	rm -vf  *.o $(PROGRAM)
