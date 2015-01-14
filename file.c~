#include<sys/types.h>
#include<stdio.h>
#include<string.h>

void main()
{
	int i, j, k;
	FILE *filefd = fopen("hello.txt", "a+");
	
	for(i = 0; i<1024; i++)
	{
		for(j = 0; j<1024; j++)
		{
			for(k = 0; k<1024; k++)
			{
				fputs("a", filefd);
			}
		}
	}
	close(filefd);
}
