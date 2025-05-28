/* THis is open system call example */
/* A program that reads a abc file content and writes in to stadard output. */

//#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
	int fd,nread,nwrite;

	char buff[100];

	printf ("argv[0]:%s\n",argv[0]);
	printf ("argv[1]:%s\n",argv[1]);

	fd = open (argv[1],O_RDONLY);
	printf ("open system call return value:%d\n",fd);
	if (fd < 0)
	{
		perror("Open FAILS\n");
		exit(0);
	}
		
	nread = read(fd,&buff,100);
	while (0 != nread)
	{
	nwrite = write(1,&buff,nread);
	nread = read(fd,&buff,100);
	}
	close(fd);
}
