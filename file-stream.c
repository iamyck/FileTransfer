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
	int clientfd;
	clientfd = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	int len = sizeof(servaddr);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(SERVER_PORT);
	if(connect(clientfd, (struct sockaddr *)&servaddr, (socklen_t)len) < 0)
	{
		printf("connect fail\n");
	};
	return clientfd;
}

int open_server()
{
	int serverfd;
	
	struct sockaddr_in servaddr;
	
	//
	serverfd = socket(PF_INET, SOCK_STREAM, 0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//bind
	if(bind(serverfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in))<0)
	{
		printf("serverfd bind fail\n");
		exit(1);
	}
	return serverfd;	
}

void request(int fd, char *filename)
{
	send(fd, filename, 10, 0);
}

int wait_for_request(int fd, char *f)
{
	
	int newclientfd;
	char *filename;
	struct sockaddr_in clientaddr;
	int len = sizeof(clientaddr);
	//listen
	listen(fd, 10);
	//accept
	newclientfd = accept(fd, (struct sockaddr*)&clientaddr, &len);	
	recv(fd, filename, 20, 0);
	//send(fd, "hello", 20, 0);
	f = filename;
	return newclientfd;
	
}
