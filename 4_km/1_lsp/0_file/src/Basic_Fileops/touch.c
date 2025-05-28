#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <utime.h>


int main()
{
	int ret;
	char buff[20];
	struct utimbuf temp;
	
/*	ret = open("kernel",O_RDONLY);
	if (ret < 0)
	{
		perror("open fails:");
		printf("errno:%d\n",errno);
	}
	else
		printf("open success ret:%d\n",ret);
*/
	utime("kernel",&temp);
	close(ret);
/*	ret = read(ret,buff,6);
	if (ret < 0)
	{
		perror("read fails:");
		printf("errno:%d\n",errno);
	}
	else
		printf("read success ret:%d\n",ret);
        ret = close(3);	
	if (ret < 0)
	{
		perror("close fails:");
		printf("errno:%d\n",errno);
	}
	else
		printf("close success ret:%d\n",ret);
	ret = open("abc",O_RDONLY);
	if (ret < 0)
	{
		perror("open fails:");
		printf("errno:%d\n",errno);
	}
	else
		printf("open success ret:%d\n",ret);
*/

}
