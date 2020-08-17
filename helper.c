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

