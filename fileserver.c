#include"file-stream.h"
#include"file-transfer.h"



void main()
{
	char *filename;
	int serverfd, controlfd;

	serverfd = open_tcp_server();
	controlfd = wait_for_request(serverfd, filename);
	printf("dfssdfsd%s\n", filename);
	//udp_file_send(controlfd, "1G.img");
	tcp_file_send(controlfd, serverfd, "1G.img");
}
