
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <time.h>
#include "dbtime.h"
#define SERVER_PORT 1555

struct controlgram
{
	int expected;
	int acget;
};

struct datagram
{
	int index;
	char buf[1024];
};


FILE *cfd;
int length = sizeof(struct datagram);
int clientfd;
int controlfd;
struct datagram sendData;
struct datagram recvData;
struct controlgram checkData;
struct sockaddr_in serveraddr;
FILE *filefd;
int len = sizeof(serveraddr);
int i;

void *reset()
{
	struct datagram reData;
	cfd = fopen("hi.txt", "r+");
	while(1)
	{
		recv(controlfd, (char *)(&reData), length, 0);
		fseek(cfd, reData.index * 1024, SEEK_SET);
		fputs(reData.buf, cfd);
		printf("fix:%d\n", reData.index);
	}
}

void main()
{
	controlfd = socket(PF_INET, SOCK_STREAM, 0);
	if(controlfd == -1)
	{
		printf("error!\n");
	}

	clientfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(clientfd == -1)
	{
		printf("error!\n");
	}
	bzero(&serveraddr, sizeof(struct sockaddr_in));
	
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(SERVER_PORT);
	printf("This is a client!\n");

	
	if(connect(controlfd, (struct sockaddr *)&serveraddr, (socklen_t)len) < 0)
	{
		printf("connect fail\n");
	};
	pthread_t tid;
	pthread_create(&tid, NULL, reset, NULL);

	sendto(clientfd, "Hello", 5, 0, (struct sockaddr *)&serveraddr, len);
	
	filefd = fopen("hi.txt", "r+");
	int i = 0;
	int size = 1024 * 1024;

	dbtime_startTest ("timing");
	while(i < size)
	{	
		recvfrom(clientfd, (char *)(&recvData), length, 0, (struct sockaddr *)&serveraddr, &len); 
		//printf("sdf:%c\n", recvBuf[bufId][7]);

		if(recvData.index != i)
		{
			if(recvData.index > i)
			{
				checkData.acget = recvData.index;
				checkData.expected = i;
				recvData.index = i;
				send(controlfd, (char *)(&checkData), sizeof(checkData), 0);
				i = recvData.index;
				fputs(recvData.buf, filefd);
			}    		
		}
		else
		{		
			i++;
			fputs(recvData.buf, filefd);
		}
		
		
	}
	dbtime_endAndShow ();

	fclose(filefd);
	printf("finish\n");
}




