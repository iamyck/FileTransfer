/*****************************************************************************/
/* Copyright (C) SSE-USTC, 2014-2015                                         */
/*                                                                           */
/*  FILE NAME             :  file-stream.h                                   */
/*  PRINCIPAL AUTHOR      :  SA14226208                                      */
/*  LANGUAGE              :  C                                               */
/*  TARGET ENVIRONMENT    :  ANY                                             */
/*  DATE OF FIRST RELEASE :  2015/01/21                                      */
/*  DESCRIPTION           :  file-stream.h                                   */
/*****************************************************************************/

#ifndef FILE_STREAM_H
#define FILE_STREAM_H

#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#define SERVER_PORT 6208          /* server port number*/

int open_client();

int open_tcp_server();

int open_udp_server();

void request(int controlfd,       /* tcp connection fd */
             char *filename);

int wait_for_request(int controlfd);

#endif /* file-stream.h */
