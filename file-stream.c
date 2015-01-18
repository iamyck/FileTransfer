#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include"file-stream.h"
#include<pthread.h>
#define SERVER_PORT 6208

int open_client()
{
	int 	clientfd;
	int 	len;
	struct 	sockaddr_in servaddr;

	len = sizeof(servaddr);
	clientfd = socket(PF_INET, SOCK_STREAM, 0);
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(SERVER_PORT);
	if(connect(clientfd, (struct sockaddr *)&servaddr, (socklen_t)len) < 0)
	{
		printf("connect fail\n");
	};
	send(clientfd, "hello", 10, 0);
	return clientfd;
}

int open_tcp_server()
{
	int 	serverfd;	
	struct 	sockaddr_in servaddr;
	
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

int open_udp_server()
{
	int 	serverfd;	
	struct 	sockaddr_in servaddr;
	
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

void request(int fd, char *filename)
{
	printf("send filemane");
	send(fd, filename, 10, 0);
}

int wait_for_request(int fd, char *f)
{
	int 	newclientfd;
	int 	len;
	char 	filename[20];
	struct 	sockaddr_in clientaddr;

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
	
	recv(newclientfd, filename, 20, 0);
	printf("%s\n", filename);
	f = filename;
	return newclientfd;
}
