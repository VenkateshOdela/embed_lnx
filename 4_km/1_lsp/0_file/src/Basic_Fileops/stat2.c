#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<stdio.h>
#include <fcntl.h>

int main()
{
	int ret,fd;
	struct stat temp;

	fd = open("abc",O_RDONLY);
	ret = fstat(fd,&temp);
	if (ret < 0)
	perror("stat Fails:");
	else
	printf("stat sucess\n");

	
	printf("size of the abc file:%ld\n",temp.st_size);

return 0;
}
