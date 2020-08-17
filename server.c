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

#include "include/helper.h"

connectionList serverConnections;

int main(int argc, char ** argv){

    struct addrinfo info_input, *destiniation;
    int yes = 1;

    info_input.ai_family = AF_INET;
    info_input.ai_protocol = IPPROTO_TCP;
    info_input.ai_flags = AI_PASSIVE;
    info_input.ai_socktype = SOCK_STREAM;

    getaddrinfo(LOCALHOST,PORT_USE,&info_input, &destiniation);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        printf("Server sockfd failed \n");
        goto clean_up;
    }

    /** This is to prevent extra errors */
    int sockfd_opt = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (sockfd_opt < 0){
        printf("Server setsockopt failed \n");
        goto clean_up;
    }

    if (bind(sockfd, destiniation->ai_addr, destiniation->ai_addrlen)){
        printf("Server failed bind \n");
    }

    if (listen(sockfd,2) < 0){
        printf("Server listen failed");
        goto clean_up;
    }

    struct sockaddr addr;
    socklen_t  addrlen = sizeof(addr);
    int new_socket;

    while((new_socket = accept(sockfd, &addr, &addrlen)) > 0){

        if (new_socket < 0 ){
        printf("new socket failed \n");
        goto clean_up;
        }

        pthread_t thread;

        if (pthread_create(& thread, NULL, connection_handler, & new_socket) < 0)
        {
            printf("Thread Failed\n");
            close(new_socket);
        }


    }

    

    char buffer[10];

    read(new_socket,&buffer,10);

    printf("%s", buffer);


    return 0;

clean_up:
    if (sockfd)
        close(sockfd);
    if (new_socket)
        close(new_socket);
    return -1;
}

