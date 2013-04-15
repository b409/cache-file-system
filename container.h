#ifndef CONTAINER_H
#define CONTAINER_H

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PORT 8888


typedef struct sender_head{
	char Connection[16];
	char Pathname[128];
}sender_head;

#endif

