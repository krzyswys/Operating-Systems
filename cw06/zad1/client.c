#include <sys/ipc.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

#include "lib.h"

int clients_queue_id = -1;
key_t clients_queue_key = -1;

int queue_descriptor = -1;
int sessionID = -1;

void send_message(Message message)
{
    if (msgsnd(queue_descriptor, &message, MSG_SIZE, 0) < 0)
    {
        fprintf(stderr, "msgsnd faliure");
    }
}

void stop()
{
    Message message = createMessage(clients_queue_key, clients_queue_id, STOP, "Stop");
    send_message(message);
    exit(0);
}

void delete_queue()
{
    if (msgctl(clients_queue_id, IPC_RMID, NULL) < 0)
    {
        fprintf(stderr, "msgctl error");
    }
    else
    {
        printf("Queue has been deleted.\n");
    }
}

void handle_sigint(int signum)
{
    stop();
}

void register_client()
{

    Message message = createMessage(clients_queue_key, clients_queue_id, INIT, "Init");
    if (msgsnd(queue_descriptor, &message, MSG_SIZE, 0) < 0)
    {
        fprintf(stderr, "sending login message to client faliure");
    }
    if (msgrcv(clients_queue_id, &message, MSG_SIZE, 0, 0) < 0)
    {
        fprintf(stderr, "login response error");
    }
    sscanf(message.content, "%d", &sessionID);
    if (sessionID < 0)
    {
        fprintf(stderr, "Queue is full");
    }

    printf("New client logged in with session ID %d.\n", sessionID);
}

int main(int argc, char **argv)
{
    atexit(delete_queue);
    signal(SIGINT, handle_sigint);

    int key = ftok(PATH, PROJECT_ID);
    if (key == -1)
    {
        fprintf(stderr, "ftok error");
    }

    queue_descriptor = msgget(key, 0);
    if (queue_descriptor < 0)
    {
        fprintf(stderr, "msgget error");
    }

    clients_queue_key = ftok(PATH, getpid());
    if (clients_queue_key == -1)
    {
        fprintf(stderr, "cannot generate clients queue key");
    }

    clients_queue_id = msgget(clients_queue_key, IPC_CREAT | IPC_EXCL | 0666);
    if (clients_queue_id < 0)
    {
        fprintf(stderr, "clients msgget");
    }

    register_client();

    char *buff;
    size_t len = 0;
    while (1)
    {
        char tokens[3] = {' ', '\n'};
        printf("\nType command:\n> ");
        getline(&buff, &len, stdin);
        char *rest;
        char *cmd = strtok_r(buff, tokens, &rest);
        if (cmd != NULL)
        {
            rest = strtok_r(rest, "\n", &rest);

            if (strcmp(cmd, "LIST") == 0)
            {
                Message message = createMessage(clients_queue_key, clients_queue_id, LIST, "List");
                send_message(message);
            }
            else if (strcmp(cmd, "2ALL") == 0)
            {
                Message message = createMessage(clients_queue_key, clients_queue_id, TALL, rest);
                send_message(message);
            }
            else if (strcmp(cmd, "2ONE") == 0)
            {
                char *num = strtok_r(rest, tokens, &rest);
                Message message = createMessage(atoi(num), clients_queue_id, TONE, rest);
                send_message(message);
            }
            else if (strcmp(cmd, "STOP") == 0)
            {
                stop();
            }

            else
            {
                printf("Command not recognized, try: [LIST,2ALL,2ONE,STOP]\n");
            }
        }
    }

    return 0;
}