#include <sys/types.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "lib.h"

int client_queue_ids[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
key_t client_queue_keys[10];
int active = 1;
int counter = 0;
int queue_descriptor = -1;
key_t server_queue_key = -1;

void log_message(Message *messageBuffer)
{
    struct tm my_time = messageBuffer->timestamp;

    FILE *result_file = fopen("logs.txt", "a");

    switch (messageBuffer->type)
    {
    case INIT:
        if (messageBuffer->client_id == -1)
        {
            fprintf(result_file, "[INIT] Max number of clients is reached!\n");
        }
        else
        {
            fprintf(result_file, "[INIT] Client id: %d\n", messageBuffer->client_id);
        }
        break;
    case LIST:
        fprintf(result_file, "[LIST] Client id: %d\n", messageBuffer->client_id);
        break;
    case TALL:
        fprintf(result_file, "Message: %s\n", messageBuffer->content);
        fprintf(result_file, "[2ALL]\n Client id: %d\n", messageBuffer->client_id);
        break;
    case TONE:
        fprintf(result_file, "Message: %s\n", messageBuffer->content);
        fprintf(result_file, "[2ONE]\n from sender id: %d,to: %d\n", messageBuffer->client_id, messageBuffer->queue_key);
        break;
    case STOP:
        fprintf(result_file, "[STOP]\n Client id: %d\n", messageBuffer->client_id);
        break;
    }

    fprintf(result_file, "sent at: %02d:%02d:%02d\n\n\n",
            my_time.tm_hour,
            my_time.tm_min,
            my_time.tm_sec);

    fclose(result_file);
}

void handle_sigint(int signum)
{
    printf("Odebrano sygnal SIGINT\n");
    active = 0;
    exit(EXIT_SUCCESS);
}

void delete_queue()
{
    if (queue_descriptor > -1)
    {
        if (msgctl(queue_descriptor, IPC_RMID, NULL) == -1)
        {
            printf("error on deleting queue");
        }
        printf("deleted servers queue successfully");
    }
}

void Init(Message *message)
{
    client_queue_ids[counter] = message->client_id;
    client_queue_keys[counter] = message->queue_key;
    if (msgget(client_queue_keys[counter], 0) == -1)
    { // to samo co na dole
        fprintf(stderr, "msgget");
    }
    if (counter >= MAX_CLIENTS)
    {
        printf("Maximum number of clients exceeded");
        sprintf(message->content, "%d", -1);
    }
    else
    {
        sprintf(message->content, "%d", counter);
        while (client_queue_ids[counter] > 0 && counter < MAX_CLIENTS)
        {
            counter++;
        }
    }
    if (msgsnd(client_queue_ids[counter - 1], message, MSG_SIZE, 0) == -1)
    {
        fprintf(stderr, "login response failed");
    }
}

void Stop(Message *message)
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (client_queue_ids[i] == message->client_id)
        {
            client_queue_ids[i] = -1;
            counter = i;
        }
    }
    printf("Client %d exited.\n", message->client_id);
}

void List()
{
    printf("Active clients:\n");
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (client_queue_ids[i] > 0)
        {
            printf("--  %d\n", client_queue_ids[i]);
        }
    }
}

void ToAll(Message *message)
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (client_queue_ids[i] > 0)
        {
            if (msgsnd(client_queue_ids[i], message, MSG_SIZE, 0) == -1)
            {
                fprintf(stderr, "ToAll msgsnd");
            }
        }
    }
}

void ToOne(Message *message)
{
    if (msgsnd(message->queue_key, message, MSG_SIZE, 0) == -1)
    {
        fprintf(stderr, "to one msgsnd");
    }
}

int main(int argc, char **argv)
{
    if (atexit(delete_queue) < 0)
    {
        fprintf(stderr, "atexit()");
    }
    if (signal(SIGINT, handle_sigint) == SIG_ERR)
    {
        fprintf(stderr, "signal()");
    }

    server_queue_key = ftok(PATH, PROJECT_ID); // generacja unikalnego klucza  - ma on byc nazwa obiektu
    if (server_queue_key == -1)
    {
        fprintf(stderr, "server ftok");
    }

    queue_descriptor = msgget(server_queue_key, IPC_CREAT | IPC_EXCL | 0666); // zwraca identyfikator kolejki komunikatów związany  zpodanym kluczem (klucz, flagi),
    if (queue_descriptor < 0)
    {
        msgctl(queue_descriptor, IPC_RMID, NULL);                                 // Usuń istniejącą kolejkę komunikatów IPC
        queue_descriptor = msgget(server_queue_key, IPC_CREAT | IPC_EXCL | 0666); // otworz ponownie
    }
    Message message; // tworzenie struktury message
    while (1)
    {
        if (active == 0 && counter == 0)
        {
            break;
        }
        if (msgrcv(queue_descriptor, &message, MSG_SIZE, 0, 0) < 0) // odbiera wiadomość z kolejki i zapisuje sa do message (id,gdzie zapisac, rozmiar,msgtyp,msgflag)
        {
            fprintf(stderr, "msgrcv");
        }
        switch (message.type)
        {
        case INIT:
            Init(&message);
            printf("Logged user with cqid %d.\n", message.client_id);
            log_message(&message);
            break;
        case STOP:
            Stop(&message);
            printf("Client %d is gone.\n", message.client_id);
            log_message(&message);
            break;
        case LIST:
            List();
            break;
        case TALL:
            ToAll(&message);
            printf("Server sent [%s] to all clients.\n", message.content);
            log_message(&message);
            break;
        case TONE:
            ToOne(&message);
            printf("Server sent [%s] to %d.\n", message.content, message.queue_key);
            log_message(&message);
            break;
        }
    }

    return 0;
}