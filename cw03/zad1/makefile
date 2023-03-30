CC=gcc
CFLAGS=-Wall -Wextra -pedantic
P_NAME= program

all: $(P_NAME)

$(P_NAME): program.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(P_NAME)
