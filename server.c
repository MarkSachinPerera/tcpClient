/**
 * Name: Server.c
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
#define CLOSE_BRACKET ": "
#define CONNECTION_STATUS "Connection Accepted"

typedef struct connectionList
{
    int socket[MAX_CLIENTS];
    int count;
} connectionList;

connectionList serverConnections;

void *connection_handler(void *socketfd);

int main(int argc, char **argv)
{

    struct addrinfo info_input, *destiniation;
    int yes = 1;
    serverConnections.count = 0;
    pthread_t thread;

    info_input.ai_family = AF_INET;
    info_input.ai_protocol = IPPROTO_TCP;
    info_input.ai_flags = AI_PASSIVE;
    info_input.ai_socktype = SOCK_STREAM;

    getaddrinfo(LOCALHOST, PORT_USE, &info_input, &destiniation);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        printf("Server sockfd failed \n");
        goto clean_up;
    }

    /** This is to prevent extra errors */
    int sockfd_opt = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (sockfd_opt < 0)
    {
        printf("Server setsockopt failed \n");
        goto clean_up;
    }

    if (bind(sockfd, destiniation->ai_addr, destiniation->ai_addrlen))
    {
        printf("Server failed bind \n");
    }

    if (listen(sockfd, 2) < 0)
    {
        printf("Server listen failed");
        goto clean_up;
    }

    struct sockaddr addr;
    socklen_t addrlen = sizeof(addr);
    int new_socket;

    while ((new_socket = accept(sockfd, &addr, &addrlen)) > 0)
    {

        if (new_socket < 0)
        {
            printf("new socket failed \n");
            goto clean_up;
        }

        if (pthread_create(&thread, NULL, connection_handler, &new_socket) < 0)
        {
            printf("Thread Failed\n");
            close(new_socket);
        }
    }

    return 0;

clean_up:
    if (sockfd)
        close(sockfd);
    if (new_socket)
        close(new_socket);
    return -1;
}

void *connection_handler(void *socketfd)
{

    int sock = *(int *)socketfd;
    char buffer[MAX_BUFFER_LEN];
    int err;
    int connectionID = 0;
    char username[MAX_BUFFER_LEN];

    if (serverConnections.count > 5)
    {
        printf("Connection refused. Max connections reached");
        return (0);
    }
    else
    {
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (serverConnections.socket[i] == 0)
            {
                connectionID = i;
                serverConnections.socket[i] = sock;
                serverConnections.count++;
                break;
            }
        }
    }
    // get username
    err = read(sock, &buffer, MAX_BUFFER_LEN);
    memset(username, 0, MAX_BUFFER_LEN);
    strncpy(username, buffer, strlen(buffer));
    strncat(username, CLOSE_BRACKET, strlen(CLOSE_BRACKET) + 1);

    printf("New user connected: %s\n", buffer);

    send(sock, CONNECTION_STATUS, MAX_BUFFER_LEN, 0);

    while ((err = read(sock, &buffer, MAX_BUFFER_LEN)) > 0)
    {

        for (int i = 0; i < serverConnections.count; i++)
        {
            if (i != connectionID)
            {
                send(serverConnections.socket[i], &username, MAX_BUFFER_LEN, 0);
                send(serverConnections.socket[i], &buffer, MAX_BUFFER_LEN, 0);
            }
        }
        memset(buffer, 0, MAX_BUFFER_LEN);
    }

    close(sock);
    serverConnections.socket[connectionID] = 0;
    serverConnections.count--;

    if (err < 0)
        printf("Error: Connection Dropped\n");
    else
        printf("Connection Closed\n");

    return (0);
}