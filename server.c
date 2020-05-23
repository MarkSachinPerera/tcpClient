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

#define LOCALHOST "127.0.0.1"
#define PORT_USE "9000"

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

    struct sockaddr * addr;
    socklen_t * addrlen;
    int new_socket = accept(sockfd, addr, addrlen);


    return 0;

clean_up:
    if (sockfd)
        close(sockfd);
    return -1;
}