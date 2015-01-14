#ifndef FILE_TRANSFER_H
#define FILE_TRANSFER_H

#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include"file-stream.h"

struct datagram
{
	int index;
	char buf[1024];	
};

struct thread_args
{
	char *filename;	
	int seek;
	int size;
	int fd;
};

void file_send(int, int, char *);

void file_get(int);

#endif /* file-transfer.h */
