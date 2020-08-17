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
#include "helper.h"


int main(int argc, char **argv)
{

    struct addrinfo info_input, *destination;

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

    if (connect(sockfd, destination->ai_addr, destination->ai_addrlen) < 0){
        printf("connect failed \n");
        goto clean_up;
    }

    send(sockfd,"hiii",strlen("hiii"),0);



    return 0;

clean_up:
    /** TODO: handle all fail cases*/
    return -1;
}