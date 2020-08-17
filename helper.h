/**
 * Name: helper.h
 * 
 * Authors: Mark Perera
 * 
 */


#define LOCALHOST "127.0.0.1"
#define PORT_USE "9000"
#define MAX_BUFFER_LEN 1024
#define MAX_CLIENTS 5

typedef struct {
    int socket[MAX_CLIENTS];
    int count;
}connectionList;

int readFromSocket(int socket, char * buffer);

int writeToSocket(int socket, char * buffer);

void * connection_handler(void * socketfd);