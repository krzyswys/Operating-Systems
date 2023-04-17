CC=gcc
CFLAGS=-Wall 

all:
	make program
	make calc

program: program.c
	$(CC) $(CFLAGS) program.c -o program

calc: calc.c
	$(CC) $(CFLAGS) calc.c -o calc

clean:
	rm -f program calc 