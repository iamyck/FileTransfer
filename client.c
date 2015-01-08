
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <time.h>
#include "dbtime.h"
#define SERVER_PORT 1555

struct datagram
{
	int index;
	char buf[1024];
};

void main()
{
	int clientfd;
	int length = sizeof(struct datagram);
	int index;
	char *indexBuf;
	struct datagram sendData;
	struct datagram recvData;
	struct sockaddr_in serveraddr;
	
	clientfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(clientfd == -1)
	{
		printf("error!\n");
	}
	bzero(&serveraddr, sizeof(struct sockaddr_in));
	
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(SERVER_PORT);
	printf("This is a client!\n");
	
	int len = sizeof(serveraddr);
	sendto(clientfd, "Hello", 5, 0, (struct sockaddr *)&serveraddr, len);
	
	FILE *filefd = fopen("hi.txt", "a+");
	int i = 0;
	int size = 1024 * 1024;

	dbtime_startTest ("timing");
	while(i < size)
	{	
		recvfrom(clientfd, (char *)(&recvData), length, 0, (struct sockaddr *)&serveraddr, &len); 
		//printf("sdf:%c\n", recvBuf[bufId][7]);

		if(recvData.index != i)
		{
			recvData.index = i;
			sendto(clientfd, (char *)(&recvData), length, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

			printf("lose:%d\n", i);
		}
		else
		{
			fputs(recvData.buf, filefd);	
			i++;
		}
		
	}
	dbtime_endAndShow ();

	fclose(filefd);
	printf("finish\n");
}




