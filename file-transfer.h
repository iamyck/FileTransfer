/*****************************************************************************/
/* Copyright (C) SSE-USTC, 2014-2015                                         */
/*                                                                           */
/*  FILE NAME             :  file-transfer.h                                 */
/*  PRINCIPAL AUTHOR      :  SA14226208                                      */
/*  LANGUAGE              :  C                                               */
/*  TARGET ENVIRONMENT    :  ANY                                             */
/*  DATE OF FIRST RELEASE :  2015/01/21                                      */
/*  DESCRIPTION           :  file-transfer.h                                 */
/*****************************************************************************/

#ifndef FILE_TRANSFER_H
#define FILE_TRANSFER_H

#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include"file-stream.h"

#define THREAD_NUM          1     /* tcp thread num */
#define TCP_DATAGRAM_SIZE   8192  /* tcp data size per package */
#define TCP_BLOCK_SIZE      128   /* tcp package num in menory buf */
#define UDP_DATAGRAM_SIZE   2048  /* udp data size per package */
#define UDP_BLOCK_SIZE      256   /* udp package num in menory buf */

/* control info */
typedef struct file_block_info
{
    int            seek;          /* start point in file */
    int            size;          /* data size of this block */
}File_block;

/* data info */
struct datagram
{
    int            index;         /* package id */
    char           buf[1024];	  /* real data */
};

/* thread args */
typedef struct thread_args
{
    char           *filename;     /* filename */	
    int            seek;          /* start point */
    int            size;          /* data size */
    int            fd;            /* socket connnetion fd */
}Thread_args;

void tcp_file_send(int controlfd, /* tcp connnetion fd */
                   int serverfd); /* tcp server fd */

void tcp_file_get(int controlfd,  /* tcp connnetion fd */
                  char *filename);/* filename */

void udp_file_send(int controlfd);/* tcp connnetion fd */

void udp_file_get(int controlfd,  /* tcp connnetion fd */
                  char *filename);/* filename */

#endif /* file-transfer.h */
