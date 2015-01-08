#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<pthread.h>
#include <time.h>
#include "dbtime.h"

#define SERVER_PORT 1555

struct datagram
{
	int index;
	char buf[1024];
};

int serverfd;
static int id = 1;
struct sockaddr_in servaddr;
struct sockaddr_in clientAddr;
struct datagram recvData;
int len = sizeof(clientAddr);
int length = sizeof(struct datagram);
int reSend;
char *indexBuf;

void *reset()
{
	while(1)
	{
		recvfrom(serverfd, (char *)(&recvData), length, 0, (struct sockaddr *)&clientAddr, &len);
		reSend = recvData.index;
		printf("dsf:%d\n", reSend);
	}
}

void main()
{
	int n;
	int index;
	char recvBuf[1024]="\0";
	struct datagram sendData;
	reSend = 0;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverfd = socket(PF_INET, SOCK_DGRAM, 0);

	if(bind(serverfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in))<0)
	{
		printf("bind fail\n");
		exit(1);
	}

	
	n = recvfrom(serverfd, recvBuf, length, 0, (struct sockaddr*)&clientAddr, &len);
	if(n < 0)
	{
		printf("recv fail");
	}
	printf("%s\n", recvBuf);
	FILE *filefd = fopen("hello.txt", "r");
	
	pthread_t tid;
	pthread_create(&tid, NULL, reset, NULL);
	
	while(fgets(sendData.buf, 1024, filefd) > 0)
	{	
		//重传
		if(reSend)
		{		
			sendData.index = reSend;
			index = reSend + 1;
			fseek(filefd, reSend * 1024, SEEK_SET);
			fgets(sendData.buf, 1024, filefd);
		}
		else
		{
			sendData.index = index;
			index++;
		}
		
		printf("send%d\n", index);
		sendto(serverfd, (char *)(&sendData), length, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
		reSend = 0;
	}
	printf("send finish\n");
}



