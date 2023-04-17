#ifndef lib_h
#define lib_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#define MAX_CLIENTS 10
#define PROJECT_ID 0x097
#define MAX_MSG_SIZE 4096
#define PATH getenv("HOME")

typedef struct Message
{
    key_t queue_key;
    int client_id;
    long type;
    char content[MAX_MSG_SIZE];
    struct tm timestamp;
} Message;

Message createMessage(key_t queue_key, int client_id, long type, char *content)
{
    Message message;
    message.queue_key = queue_key;
    message.client_id = client_id;
    message.type = type;
    sprintf(message.content, "%s", content);
    time_t now = time(NULL);
    message.timestamp = *localtime(&now);

    return message;
}

typedef enum MessageTypeE
{
    INIT = 1,
    LIST = 2,
    TONE = 3,
    TALL = 4,
    STOP = 5
} MessageTypeE;

const size_t MSG_SIZE = sizeof(Message);

#endif