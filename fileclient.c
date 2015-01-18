#include"file-stream.h"
#include"file-transfer.h"

void main()
{
	int fd;
	fd = open_client();	
	request(fd, "1G.img");	
	tcp_file_get(fd);
	//udp_file_get(fd);
}
