#ifndef FILE_TRANSFER_H
#define FILE_TRANSFER_H

#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include"file-stream.h"

/* control info */
struct controlgram
{
	int 	begin;
	int 	size;
	unsigned char 	map[256];
};

/* data info */
struct datagram
{
	int 	begin;
	int 	index;
	char 	buf[1024];	
};

/* thread args */
struct thread_args
{
	char 	*filename;	
	int 	seek;
	int 	size;
	int 	fd;
};

void tcp_file_send(int, int, char *);

void tcp_file_get(int);

void udp_file_send(int , char *);

void udp_file_get(int);

#endif /* file-transfer.h */
