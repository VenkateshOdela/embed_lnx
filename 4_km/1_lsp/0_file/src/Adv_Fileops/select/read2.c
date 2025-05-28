#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <linux/input.h>

int main()
{
	int fd1,retfd;
	char buf[4096];
	int val;
	char name[256] = "Unknown";

	fd1=open("/dev/input/event0",O_RDONLY);
	if(fd1<0)
	{
		perror("open Fails");
		return -1;
	}
        ioctl(fd1, EVIOCGNAME(sizeof(name)), name);
        printf("Input device name: \"%s\"\n", name);

	
	retfd=read(fd1,buf,4096);
	printf("Read return value - fd1 - :%d \n",retfd);
	if(retfd<0){
		printf("read is failed\n");
		return -1;
		}

	printf("buf- :%s \n",buf);
	return 0;
}
		
