/**
 * Name: Client.c
 * 
 * Authors: Mark Perera
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syscall.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <assert.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

#define LOCALHOST "127.0.0.1"
#define PORT_USE "9000"
#define MAX_BUFFER_LEN 1024
#define MAX_CLIENTS 5

int communicate(int socket);
void *recieve(void *socket);

int main(int argc, char **argv)
{

    struct addrinfo info_input, *destination;
    char username[MAX_BUFFER_LEN];
    pthread_t thread;

    if (argc != 2)
    {
        printf("Usage: ./client [username]\n");
        return (0);
    }

    strncpy(username, argv[1], strlen(argv[1]));

    info_input.ai_family = AF_INET;
    info_input.ai_socktype = SOCK_STREAM;
    info_input.ai_protocol = IPPROTO_TCP;
    info_input.ai_flags = AI_PASSIVE;

    getaddrinfo(LOCALHOST, PORT_USE, &info_input, &destination);
    /**
     * This will create the path to where to send the data
     */

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /** 
     * The socket() function shall create an unbound socket 
     * in a communications domain 
     * */
    if (sockfd == -1)
    {
        printf("Client failed to get sockfd \n");
        goto clean_up;
    }

    if (connect(sockfd, destination->ai_addr, destination->ai_addrlen) < 0)
    {
        printf("connect failed \n");
        goto clean_up;
    }

    send(sockfd, username, strlen(username), 0);

    if (pthread_create(&thread, NULL, recieve, &sockfd) < 0)
    {
        printf("Error: Thread Create Failed \n");
        goto clean_up;
    }

    if (communicate(sockfd))
        goto clean_up;

clean_up:
    if (sockfd)
        close(sockfd);
    return -1;
}

void *recieve(void *socket)
{
    char *buffer = NULL;
    int err;
    int sockfd = *(int *)socket;
    buffer = malloc(MAX_BUFFER_LEN * sizeof(char));

    while (1)
    {
        err = read(sockfd, &buffer, MAX_BUFFER_LEN);
        if (err <= 0)
        {
            printf("Connection Closed\n");
            break;
        }
        printf("%s\n", buffer);
    }
    free(buffer);

    return (0);
}

int communicate(int socket)
{

    char *buffer = NULL;
    int err;
    size_t buffersize = MAX_BUFFER_LEN;
    size_t bufferCount = 0;

    buffer = malloc(MAX_BUFFER_LEN * sizeof(char));
    printf("> [Message] \n");
    while (1)
    {

        printf("> ");
        bufferCount = getline(&buffer, &buffersize, stdin);
        if (bufferCount != 0)
        {
            err = send(socket, buffer, bufferCount, 0);
            memset(buffer, 0, MAX_BUFFER_LEN);

            if (err <= 0)
            {
                printf("Connection Closed\n");
                break;
            }
        }
    }
    if (socket)
        close(socket);

    free(buffer);

    return (err);
}