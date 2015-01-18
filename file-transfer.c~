#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include"file-stream.h"
#include"file-transfer.h"
#include<pthread.h>
#include<sys/epoll.h>  

#define THREAD_NUM 1
#define MAX_EVENTS 10
#define DATAGRAM_SIZE 1024
#define BLOCK_SIZE 256

/* tcp send thread fuction */
void *tcp_send_thread(void *a)
{	
	char 	*finish;
	int 	fd;
	int 	size;
	int 	seek;	
	int 	length;
	int 	index;
	void    *fp;
	void 	*cur;
	FILE 	*filefd;
	struct 	datagram sendData;
	struct 	thread_args *args;
	
	fp = malloc(1024*256);
	args = (struct thread_args *)a;	
	fd = args->fd;
	size = args->size;
	seek = args->seek;
	length = sizeof(sendData);
	index = 0;

	//printf("%s\n", args->filename);
	filefd = fopen(args->filename, "r");
	fseek(filefd, seek*1024, SEEK_SET);
	send(fd, (char *)(&seek), 4, 0);
	
	while(fread(fp, 1024*256, 1, filefd)>0)
	{
		cur = fp;
		for(index = 0; index < 256; index++)
		{
			send(fd, cur, 1024, 0);
		}
		recv(fd, (char*)&seek, 4, 0);
		bzero(fp, 1024*256);	
	}
	/* send last one */
	sendData.index = -1;
	send(fd, (char *)(&sendData), length, 0);
	close(filefd);
	close(fd);	
}

/* tcp receive thread fuction */
void *tcp_recv_thread(void *a)
{	
	int 	newfd;
	int 	seek;
	int 	length;
	int		i;
	void 	*fp;
	void 	*cur;
	FILE 	*filefd;
	struct	datagram recvData;
	struct 	thread_args *args;
	
	fp = malloc(1024*256);
	cur = fp;
	args = (struct thread_args *)a;
	newfd = args->fd;
	length = sizeof(recvData);

	filefd = fopen("hi.img", "r+");
	recv(newfd, (char*)&seek, 4, 0);
	fseek(filefd, seek*1024, SEEK_SET);
	/* receive */
	for(;;)
	{
		for(i = 0; i < 256; i++)
		{
			recv(newfd, cur, 1024, 0);
			cur += 1024;
		}	
		send(newfd, "4", 2, 0);
		fwrite(fp, 1024*256, 1, filefd);
		bzero(fp, 1024*256);
		cur = fp;
	}
}

/* send file using tcp */
void tcp_file_send(int controlfd, int serverfd, char *filename)
{
	
	char 	*finish;
	int 	len;
	int 	length;
	int 	i;
	int 	size;
	struct 	datagram sendData;
	struct 	sockaddr_in clientaddr;
	pthread_t tid[10];
	
	len = sizeof(clientaddr);
	length = sizeof(sendData);
	size = (1073741824 / 1024 + 2000) / THREAD_NUM + 1;
	
	for(i = 0; i<THREAD_NUM; i++)
	{
		listen(serverfd, 10);
		struct thread_args *args;
		args = (struct thread_args *)malloc(sizeof(struct thread_args));
		/* accept */
		int newclientfd = accept(serverfd, (struct sockaddr*)&clientaddr, &len);
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
}

/* get file using tcp */
void tcp_file_get(int fd)
{
	/* time start */
	time_t t_start,t_end;
   	t_start = time(NULL);

	int 	i;
	int 	length;
	char 	*finish;
	FILE 	*filefd;
	struct 	datagram recvData;

	length = sizeof(recvData);
	filefd = fopen("hi.img", "w+");
	close(filefd);

	pthread_t tid[10];
	/* open threads */
	for(i = 0; i<THREAD_NUM; i++)
	{
		struct thread_args *args;
		args = (struct thread_args *)malloc(sizeof(struct thread_args));
		int newfd = open_client();
		args->fd = newfd;
		args->size = fd;
		pthread_create(&(tid[i]), NULL, tcp_recv_thread, (void *)args);
	}

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

void udp_file_send(int controlfd, char *filename)
{	
	send(controlfd, "hello", 10, 0);
	int 	begin;
	int 	index;
	int 	serverfd;
	int 	len;
	int 	i;
	int 	lost;
	int 	read_size;
	int		pack_count;
	void	*fp;
	void	*send_point;
	FILE 	*send_file_fd;
	struct 	sockaddr_in clientaddr;
	struct 	datagram sendData;
	struct	controlgram controlData;
	
	begin = 0;
	index = 0;
	fp = malloc(1024*256);
	serverfd = open_udp_server();
	send_file_fd = fopen("1G.img", "r");

	char buf[20];
	len = sizeof(clientaddr);
	recvfrom(serverfd, buf, 20, 0,
			(struct sockaddr*)&clientaddr, &len);
	printf("%s\n", buf);
	
	/* time start */
	time_t t_start,t_end;
	t_start=time(NULL);

	while(1)
	{
		bzero(fp, 1024*256);
		send_point = fp;
		read_size = fread(fp, 1024*256, 1, send_file_fd);

		/* send data */
		pack_count = 0;
		i = 255;
		while(read_size > 0)
		{
			sendData.index = pack_count;
			memcpy(sendData.buf, send_point, 1024);		
			sendto(serverfd, (void*)&sendData, sizeof(sendData), 0, 
					(struct sockaddr*)&clientaddr,sizeof(clientaddr));
			printf("send %d\n", pack_count);
			pack_count++;
			i--;
		}

		/* one block check */
		sendData.index = -1;
		send(controlfd, (void*)&sendData, sizeof(sendData), 0);
		recv(controlfd, (void *)&controlData, sizeof(controlData), 0);

		/* resend lost data */
		for(i = 0; i < pack_count; i++)
		{
			if(controlData.map[i] == 0)
			{
				sendData.index = pack_count;
				memcpy(sendData.buf, fp + pack_count*1024, 1024);
				send(controlfd, (void*)&sendData, sizeof(sendData), 0);
				printf("resend%d\n", i);
			}
		}	

		/* one block last */
		sendData.index = -2;
		send(controlfd, (void*)&sendData, sizeof(sendData), 0);
		recv(controlfd, (void *)&controlData, sizeof(controlData), 0);
		/* send finish */
		if(read_size == 0)
		{
			sendData.index = -3;
			send(controlfd, (void*)&sendData, sizeof(sendData), 0);
			printf("finish\n");
			break;
		}
	}

	/* time end */
	t_end=time(NULL);
   	printf("共用时%.0fs\n", (double)difftime(t_end,t_start));	
}

void udp_file_get(int controlfd)
{
	char 	buf[1024];
	recv(controlfd, buf, 1024, 0);
	printf("%s\n", buf);
	int		cur_begin;
	int 	cur_index;
	int		err_size;
	int		gap_size;
	int		i;
	int 	len;
	int 	clientfd;
	int		epoll_fd;
	int		epoll_ev;
	void	*fp;
	FILE 	*recv_file_fd;
	struct 	sockaddr_in serveraddr;
	struct 	datagram recvData;
	struct	controlgram controlData;
	struct 	epoll_event ev; 
	struct 	epoll_event events[MAX_EVENTS];

	cur_begin = 0;
	cur_index = 0;
	err_size = 0;
	bzero(controlData.map, 256);
	len = sizeof(serveraddr);
	fp = malloc(1024*256);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(6208);

	clientfd = socket(PF_INET, SOCK_DGRAM, 0);
	recv_file_fd = fopen("hi.img", "w+");
	sendto(clientfd, "hello", sizeof("hello"), 0, 
		(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	
	/* epoll open */
	epoll_fd = epoll_create(MAX_EVENTS);
	if(epoll_fd == -1)  
	{  
		perror("epoll create failed");  
		exit(1);  
	}

	/* add client event */
	ev.events = EPOLLIN;  
	ev.data.fd = clientfd;
	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientfd, &ev) == -1)  
	{  
		perror("epll_ctl:clientfd register failed");
		exit(1);  
	}
	
	/* add control event */
	ev.events = EPOLLIN;  
	ev.data.fd = controlfd;
	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, controlfd, &ev) == -1)  
	{  
		perror("epll_ctl:controlfd register failed");  
		exit(1);  
	}
		
	for(;;)
	{
		epoll_ev = epoll_wait(epoll_fd,events, MAX_EVENTS, -1);
		if(epoll_ev == -1)
		{  
			perror("start epoll_wait failed");  
			exit(1);  
		}
		/* handle event */
		for(i = 0; i<epoll_ev; i++)  
		{  
			/* contorl data arrive */ 
			if(events[i].data.fd == controlfd)  
			{  
				recv(controlfd, (void*)&recvData, sizeof(recvData), 0);
				switch(recvData.index)
				{
					/* one udp finish */
					case -1:		
						send(controlfd, (void*)&controlData, sizeof(controlData), 0);
						for(i = 0; i < 256; i++)	
						{
							printf("map[%d]:%d\n", i, controlData.map[i]);
						}
						break;

					/* one block finish */
					case -2:
						fwrite(fp, 1024*256, 1, recv_file_fd);
						bzero(controlData.map, 256);
						send(controlfd, "hello", 10, 0);
						break;

					/* all finish */
					case -3:
						close(controlfd);
						return;

					/* resend data */
					default:
						memcpy(fp+recvData.index*1024, recvData.buf, 1024);
						break;	
				}
			}  
 			/* fiel data arrive*/ 
			else if(events[i].data.fd == clientfd)
			{
				recvfrom(clientfd, (unsigned char*)&recvData, sizeof(recvData), 0,
					(struct sockaddr*)&serveraddr, &len);
				controlData.map[recvData.index] = 1;
				memcpy(fp+recvData.index*1024, recvData.buf, 1024);
				printf("recv%s\n",(char*)fp);
				printf("get:%d\n", recvData.index);
			}
		}	
	}
}

