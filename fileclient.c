#include"file-stream.h"
#include"file-transfer.h"
#include"dbtime.h"


void main()
{
	int fd;
	
	fd = open_client();
	
	request(fd, "hello.txt");
	dbtime_startTest ("timing");
	tcp_file_get(fd);
	dbtime_endAndShow ();
}
