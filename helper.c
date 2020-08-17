/**
 * Name: helper.c
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


#include "include/helper.h"

extern connectionList serverConnections;

void * connection_handler(void *socketfd)
{

    int sock = *(int *)socketfd;
    char buffer[MAX_BUFFER_LEN];
    int err;
    int connectionID = 0;
    char username[MAX_BUFFER_LEN];

    if (serverConnections.count > 5)
        return(0);
    else
    {
        for (int i = 0; i < 5; i++)
        {
            if (serverConnections.socket[i] == 0)
            {
                connectionID = i;
                serverConnections.socket[i] = sock;
                serverConnections.count++;
            }
        }
    }

    // get username
    err = recv(sock, &buffer, MAX_BUFFER_LEN, 0);
    strncpy(username, buffer, MAX_BUFFER_LEN);

    printf("New user connected: %s",username);

    while ((err = recv(sock, &buffer, MAX_BUFFER_LEN, 0)) > 0)
    {
        break;
    }

    close(sock);
    serverConnections.socket[connectionID] = 0;
    serverConnections.count--;

    if (err < 0)
        printf("Error: Connection Dropped");
    else
        printf("Connection Closed");

    return(0);
}