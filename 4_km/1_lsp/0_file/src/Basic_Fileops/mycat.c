#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	int ret;
	char buff[20];
	
	ret = open(argv[1],O_RDONLY);
	if (ret < 0)
	{
		perror("open fails:");
		printf("errno:%d\n",errno);
	}
	else
		printf("open success ret:%d\n",ret);
	ret = read(ret,buff,6);
	if (ret < 0)
	{
		perror("read fails:");
		printf("errno:%d\n",errno);
	}
	else
		printf("read success ret:%d\n",ret);
/*	ret = close(3);	
	if (ret < 0)
	{
		perror("close fails:");
		printf("errno:%d\n",errno);
	}
	else
		printf("close success ret:%d\n",ret);
*/
	ret = open("abc",O_RDONLY);
	if (ret < 0)
	{
		perror("open fails:");
		printf("errno:%d\n",errno);
	}
	else
		printf("open success ret:%d\n",ret);
}
