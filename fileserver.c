#include"file-stream.h"
#include"file-transfer.h"



void main()
{
	char *filename;
	int serverfd, controlfd;
	serverfd = open_server();

	controlfd = wait_for_request(serverfd, filename);
	
	tcp_file_send(controlfd, serverfd, "hello.txt");
}
