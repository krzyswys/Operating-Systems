CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11
LDFLAGS=
P_NAME=program

all: $(P_NAME)

$(P_NAME): program.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(P_NAME)