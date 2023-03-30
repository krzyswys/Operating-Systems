CC = gcc
CFLAGS = -Wall
P1_NAME = program
P2_NAME = exec
all: $(P1_NAME) $(P2_NAME)

$(P1_NAME): program.c
	$(CC) $(CFLAGS) program.c -o $(P1_NAME)

$(P2_NAME): exec.c
	$(CC) $(CFLAGS) exec.c -o $(P2_NAME)

clean:
	rm -f $(P1_NAME) $(P2_NAME)