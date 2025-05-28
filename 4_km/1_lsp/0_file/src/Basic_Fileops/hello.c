#include<unistd.h>
#include<stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int main()
{
	int fd,ret;
	char buff[10];

	fd = open("/home/kernel/Desktop/abc",O_RDONLY);
	if (fd < 0){
	perror("open Fails:");
	printf("errorno:%d\n",errno);
	}

	
	ret = read(fd,buff,10);
	if (ret < 0){
	perror("Read Fails:");
	printf("errorno:%d\n",errno);
	}

	write(1,buff,10);
	close(fd);
	fd = open("/home/kernel/Desktop/xyz",O_RDONLY);
	if (fd < 0){
	perror("open Fails:");
	printf("errorno:%d\n",errno);
	}



	return 0;
}
