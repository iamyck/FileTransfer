/*****************************************************************************/
/* Copyright (C) SSE-USTC, 2014-2015                                         */
/*                                                                           */
/*  FILE NAME             :  file-stream.c                                   */
/*  PRINCIPAL AUTHOR      :  SA14226208                                      */
/*  LANGUAGE              :  C                                               */
/*  TARGET ENVIRONMENT    :  ANY                                             */
/*  DATE OF FIRST RELEASE :  2015/01/21                                      */
/*  DESCRIPTION           :  net initial functions                           */
/*****************************************************************************/

#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include"file-stream.h"
#include<pthread.h>

/* create a client tcp fd */
int open_client()
{
    int            clientfd;      /* client tcp fd*/
    int            len;           /* sockaddr length*/
    struct sockaddr_in servaddr;  /*server addr*/

    len = sizeof(servaddr);
    clientfd = socket(PF_INET, SOCK_STREAM, 0);
	
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(SERVER_PORT);
    if(connect(clientfd, (struct sockaddr *)&servaddr, (socklen_t)len) < 0)
    {
        printf("connect fail\n");
    };
    return clientfd;
}

/* create a server tcp fd */
int open_tcp_server()
{
    int            serverfd;      /* tcp server fd */
    struct sockaddr_in servaddr;  /* tcp server addr */
	
    serverfd = socket(PF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* bind */
    if(bind(serverfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in))<0)
    {
        printf("serverfd bind fail\n");
        exit(1);
    }
    return serverfd;	
}

/* create a server udp fd */
int open_udp_server()
{
    int            serverfd;      /* udp server fd */	 
    struct sockaddr_in servaddr;  /* udp server addr */
	
    serverfd = socket(PF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* bind */
    if(bind(serverfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in))<0)
    {
        printf("serverfd bind fail\n");
        exit(1);
    }
    return serverfd;	
}

/* send file name */
void request(int fd, char *filename)
{
    printf("send filemane");
    send(fd, filename, 10, 0);
}


/* server get a tcp connection */
int wait_for_request(int fd)
{
    char           filename[20];  /* filename */
    int            newclientfd;   /* tcp new connection fd */
    int            len;           /* sockaddr length */
    struct sockaddr_in clientaddr;

    len = sizeof(clientaddr);

    /* listen */
    listen(fd, 10);

    /* accept */
    newclientfd = accept(fd, (struct sockaddr*)&clientaddr, &len);
    if(newclientfd < 0)
    {
        printf("accept error\n");
        exit(1);
    }	
    return newclientfd;
}
