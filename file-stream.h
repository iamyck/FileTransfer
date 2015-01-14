#ifndef FILE_STREAM_H
#define FILE_STREAM_H

#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int open_client();

int open_server();

void request(int, char *);

int wait_for_request(int, char *);

#endif /* file-stream.h */
