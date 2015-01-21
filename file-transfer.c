/*****************************************************************************/
/* Copyright (C) SSE-USTC, 2014-2015                                         */
/*                                                                           */
/*  FILE NAME             :  file-transfer.c                                 */
/*  PRINCIPAL AUTHOR      :  SA14226208                                      */
/*  LANGUAGE              :  C                                               */
/*  TARGET ENVIRONMENT    :  ANY                                             */
/*  DATE OF FIRST RELEASE :  2015/01/21                                      */
/*  DESCRIPTION           :  transfer functions                              */
/*****************************************************************************/

#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<pthread.h>
#include"file-stream.h"
#include"file-transfer.h"

/* tcp send thread fuction */
void *tcp_send_thread(void *a)
{	
    int            sendfd;        /* tcp connection fd */
    int            size;          /* file block size */
    int            seek;          /* file start point */
    int            i;             /* for loop value */
    int            mem_buf_size;  /* memory buffer size */
    void           *fp;           /* memory buffer point */
    void           *cur;          /* current point */
    FILE           *filefd;       /* sended file fd */
    Thread_args    *args;         /* thread function args */
    File_block     block_info;    /* sended file block info */

    /* pick out formals */
    args = (struct thread_args *)a;	
    sendfd = args->fd;
    size = args->size;
    seek = args->seek;	
   
    /* malloc */
    mem_buf_size = TCP_DATAGRAM_SIZE * TCP_BLOCK_SIZE;
    fp = malloc(mem_buf_size);  
  
    /* open file send block info */ 
    filefd = fopen(args->filename, "r");
    fseek(filefd, seek * TCP_DATAGRAM_SIZE, SEEK_SET);
    block_info.seek = seek;
    block_info.size = size/TCP_BLOCK_SIZE;
    send(sendfd, (char *)(&block_info), sizeof(block_info), 0);
	
    /* send loop */
    while(fread(fp, mem_buf_size, 1, filefd) > 0)
    {
        cur = fp;
        for(i = 0; i < TCP_BLOCK_SIZE; i++)
        {
            send(sendfd, cur, TCP_DATAGRAM_SIZE, 0);
            cur += TCP_DATAGRAM_SIZE;
            recv(sendfd, (char *)(&seek), 4, 0);
        }
        bzero(fp, TCP_DATAGRAM_SIZE * TCP_BLOCK_SIZE);	
    }

    close(filefd);
    close(sendfd);	
}

/* tcp receive thread fuction */
void *tcp_recv_thread(void *a)
{	
    int            recvfd;        /* tcp connection fd */
    int            seek;          /* file block size */
    int            recv_len;      /* received length */    
    int            pack_num;      /* number of packages */ 
    int            mem_buf_size;  /* memory buffer size */
    int            i;             /* for loop value */
    int            j;             /* for loop value */
    void           *fp;           /* memory buffer point */
    void           *cur;          /* current point */
    FILE           *filefd;       /* written file fd */
    Thread_args    *args;         /* thread function args */
    File_block     block_info;    /* sended file block info */
	
    mem_buf_size = TCP_DATAGRAM_SIZE * TCP_BLOCK_SIZE;
    fp = malloc(mem_buf_size);
    cur = fp;
    args = (struct thread_args *)a;
    recvfd = args->fd;
	
    /* open written file */
    filefd = fopen("hi.img", "r+");

    /* recv block info and do preparation */
    recv(recvfd, (char*)&block_info, sizeof(block_info), 0);
    seek = block_info.seek;
    pack_num = block_info.size;
    fseek(filefd, seek * TCP_DATAGRAM_SIZE, SEEK_SET);

    /* receive */
    for(j = 0; j < pack_num; j++)
    {
        for(i = 0; i < TCP_BLOCK_SIZE; i++)
        {
            recv_len = recv(recvfd, cur, TCP_DATAGRAM_SIZE, 0);
            cur += TCP_DATAGRAM_SIZE;
            send(recvfd, "4", 2, 0);
        }	
        fwrite(fp, mem_buf_size, 1, filefd);
        bzero(fp, mem_buf_size);
        cur = fp;
    }

    close(filefd); 
    close(recvfd);
}

/* send file using tcp */
void tcp_file_send(int controlfd, int serverfd)
{
    char           filename[20];
    int            len;           /* size of sockaddr_in */
    int            i;             /* for loop value */
    int            size;          /* data size per thread */
    int            newclientfd;   /* new tcp connection fd */
    pthread_t      tid[10];       /* thread id array */
    struct sockaddr_in clientaddr;/* client socket addr */
    
    len = sizeof(clientaddr);

    /* time start */
    time_t t_start,t_end;
    t_start = time(NULL);
	
    /* recv file name */
    recv(controlfd, filename, 20, 0);
    printf("%s\n", filename);
    size = 1073741824 / TCP_DATAGRAM_SIZE;
	
    for(i = 0; i<THREAD_NUM; i++)
    {
        struct thread_args *args;
        listen(serverfd, 10); 
        args = (struct thread_args *)malloc(sizeof(struct thread_args));

        /* accept */
        newclientfd = accept(serverfd, (struct sockaddr*)&clientaddr, &len);
        args->filename = filename;
        args->seek = i * size;	
        args->size = size;
        args->fd = newclientfd;

        /* open thread */
        pthread_create(&(tid[i]), NULL, tcp_send_thread, (void *)args);		
    }
	
    /* wait threads */
    for(i = 0; i<THREAD_NUM; i++)
    {
        pthread_join(tid[i], NULL);
    }
    close(controlfd);
    printf("finish\n");
	
    /* time end */
    printf("finish\n");
    t_end = time(NULL);
    printf("共用时%.0fs\n", (double)difftime(t_end,t_start));	
}

/* get file using tcp */
void tcp_file_get(int fd, char *filename)
{
    int            i;             /* for loop value */
    FILE           *filefd;       /* written file fd */
    pthread_t      tid[10];       /* thread id array */

    filefd = fopen("hi.img", "w+");
    close(filefd);

    /* time start */
    time_t t_start,t_end;
    t_start = time(NULL);

    /* request */
    request(fd, filename);

    /* open threads */
    for(i = 0; i<THREAD_NUM; i++)
    {
        int newfd = open_client();
        struct thread_args *args;
        args = (struct thread_args *)malloc(sizeof(struct thread_args));  
        args->fd = newfd;
        args->size = fd;
        pthread_create(&(tid[i]), NULL, tcp_recv_thread, (void *)args);
    }

    /*wait for thread */
    for(i = 0; i<THREAD_NUM; i++)
    {
    pthread_join(tid[i], NULL);
    }
    close(fd);

    /* time end */
    printf("finish\n");
    t_end = time(NULL);
    printf("共用时%.0fs\n", (double)difftime(t_end,t_start));	
}

void udp_file_send(int controlfd)
{	
    char           filename[20];  /* filename */
    int            udp_serverfd;  /* udp server fd */  
    int            len;           /* sizeof client addr */
    int            i;             /* for loop value */
    int            mem_buf_size;  /* memory buffer size */
    void           *fp;           /* memory buffer point */
    void           *cur;          /* current point */
    FILE           *send_file_fd; /* sended file fd */
    struct sockaddr_in clientaddr;/* client addr */
	
    /* malloc */
    mem_buf_size = UDP_DATAGRAM_SIZE * UDP_BLOCK_SIZE;
    fp = malloc(mem_buf_size);

    /* open udp server */
    udp_serverfd = open_udp_server();
    len = sizeof(clientaddr);
	
    /* time start */
    time_t t_start,t_end;
    t_start=time(NULL);
	
    /* recv file name and open */
    recvfrom(udp_serverfd, filename, 20, 0,
             (struct sockaddr*)&clientaddr, &len);
    printf("filename%s\n", filename);
    send_file_fd = fopen(filename, "r");

    /* send */
    while(fread(fp, mem_buf_size, 1, send_file_fd)>0)
    {
        cur = fp;
        for(i = 0; i < UDP_BLOCK_SIZE; i++)
        {
            sendto(udp_serverfd, cur, 1024, 0, 
                   (struct sockaddr*)&clientaddr,sizeof(clientaddr));
            cur += UDP_DATAGRAM_SIZE;
            recvfrom(udp_serverfd, filename, 20, 0,
                   (struct sockaddr*)&clientaddr, &len);
        }
        bzero(fp, mem_buf_size);	
    }

    close(send_file_fd);
    close(udp_serverfd);	
	
    /* time end */
    t_end=time(NULL);
    printf("共用时%.0fs\n", (double)difftime(t_end,t_start));	
}

void udp_file_get(int controlfd, char *filename)
{
    int            i;             /* for loop value */
    int            len;           /* sizeof server addr */
    int            clientfd;      /* receive udp fd */   
    int            mem_buf_size;  /* memory buffer size */
    void           *fp;           /* memory buffer point */
    void           *cur;          /* current point */
    FILE           *recv_file_fd; /* written file fd */
    struct sockaddr_in serveraddr;

    recv_file_fd = fopen("hi.img", "w+");
    len = sizeof(serveraddr);
    /* malloc */
    mem_buf_size = UDP_DATAGRAM_SIZE * UDP_BLOCK_SIZE;
    fp = malloc(mem_buf_size);
    cur = fp;

    /* udp initial */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(6208);
    clientfd = socket(PF_INET, SOCK_DGRAM, 0);

    /* send filename */
    sendto(clientfd, filename, 20, 0,
        (struct sockaddr*)&serveraddr,sizeof(serveraddr));

    /* recv */
    while(1)
    {	
        for(i = 0; i < UDP_BLOCK_SIZE; i++)
        {
            recvfrom(clientfd, cur, UDP_DATAGRAM_SIZE, 0,
                (struct sockaddr*)&serveraddr, &len);
            cur += UDP_DATAGRAM_SIZE;
            sendto(clientfd, "1G.img", 20, 0, 
                (struct sockaddr*)&serveraddr,sizeof(serveraddr));		
        }	
			
        fwrite(fp, UDP_DATAGRAM_SIZE * UDP_BLOCK_SIZE, 1, recv_file_fd);
        bzero(fp, UDP_DATAGRAM_SIZE * UDP_BLOCK_SIZE);
        cur = fp;
    }
}

