CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -D_POSIX_C_SOURCE=200809L
P_NAME = program

all: $(P_NAME)

$(P_NAME): program.c
	$(CC) $(CFLAGS) program.c -o $(P_NAME)

clean:
	rm -f $(P_NAME)

