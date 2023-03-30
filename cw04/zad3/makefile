CC = gcc
CFLAGS = -Wall
P1_NAME = sender
P2_NAME = catcher

F1_NAME=sender.c
F2_NAME= catcher.c

all: $(P1_NAME) $(P2_NAME)

$(P1_NAME): $(F1_NAME)
	$(CC) $(CFLAGS) $(F1_NAME) -o $(P1_NAME)

$(P2_NAME): $(F2_NAME)
	$(CC) $(CFLAGS) $(F2_NAME) -o $(P2_NAME)

clean:
	rm -f $(P1_NAME) $(P2_NAME) catcher_pid.txt