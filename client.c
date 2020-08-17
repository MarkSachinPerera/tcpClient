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

#define LOCALHOST "127.0.0.1"
#define PORT_USE "9000"
#define MAX_BUFFER_LEN 1024
#define MAX_CLIENTS 5

int main(int argc, char **argv)
{

    struct addrinfo info_input, *destination;
    char buffer[MAX_BUFFER_LEN];
    int err;
    /** TODO: Usage */

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

    send(sockfd, "Mark", strlen("Mark"), 0);

    while (1)
    {
        // err = read(sockfd, &buffer, MAX_BUFFER_LEN);
        printf("Usage: [username] [Message] \n");
        scanf("%s", buffer);
        send(sockfd, &buffer, MAX_BUFFER_LEN, 0);
    }
    close(sockfd);

    if (err < 0)
        printf("Error: Connection Dropped\n");
    else
        printf("Connection Closed\n");

    return 0;

clean_up:
    /** TODO: handle all fail cases*/
    return -1;
}