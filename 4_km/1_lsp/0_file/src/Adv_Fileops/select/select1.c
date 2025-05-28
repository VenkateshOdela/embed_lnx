#if 0
#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<linux/input.h>

int main()
{
	fd_set read_fileset;
	int fd1,fd2, retfd;
	int buf;
	int val,ret;
	char buff[4099];
	struct input_event ev;

	fd1=open("/dev/input/event1",O_RDONLY); // Mouse
	if(fd1<0)
		printf("error while open the file event1\n");
	fd2=open("/dev/input/event0",O_RDONLY);// Keyboard
	if(fd2<0)
		printf("error while open the file event2\n");
	
	/* initially makes as zero files */
	FD_ZERO(&read_fileset);

	printf("Max. no of fd's:%d\n",FD_SETSIZE);
	while(1){
		/* add our file discriptors */
		FD_SET(fd1,&read_fileset);
		FD_SET(fd2,&read_fileset);
		retfd=select(FD_SETSIZE,&read_fileset,NULL,NULL,0);
		printf ("retfd:%d\n",retfd);
		if(retfd<0){
				printf("select is failed\n");
			return -1;
		}
		/* Mouse data event0  */
		if(FD_ISSET(fd1,&read_fileset)){
			printf("data is available on mouse event0\n");
			ret = read(fd1,&ev,sizeof(struct input_event));
        		printf("ret:%d ev.type:%d ev.code:%d ev.value:%d\n",ret,ev.type,ev.code,ev.value);
		}
		/* Keypad event1  */
		if(FD_ISSET(fd2,&read_fileset)){
			printf("Keyboard event\n");
			ret = read(fd2,&ev,sizeof(struct input_event));
        		printf("ret:%d ev.type:%d ev.code:%d ev.value:%d\n",ret,ev.type,ev.code,ev.value);
		}
	}
	return 0;
}
#endif
#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<linux/input.h>

int main()
{
	fd_set read_fileset;
	int fd1,fd2, retfd;
	int buf;
	int val,ret;
	char buff[4099];
	struct input_event ev;

	fd1=open("/dev/input/event1",O_RDONLY); // Mouse
	if(fd1<0)
		printf("error while open the file event1\n");
	fd2=open("/dev/input/event0",O_RDONLY);// Keyboard
	if(fd2<0)
		printf("error while open the file event2\n");

	/* initially makes as zero files */
	FD_ZERO(&read_fileset);

	printf("Max. no of fd's:%d\n",FD_SETSIZE);
	while(1){
		/* add our file discriptors */
		FD_SET(fd1,&read_fileset);
		FD_SET(fd2,&read_fileset);
		retfd=select(FD_SETSIZE,&read_fileset,NULL,NULL,0);
		printf ("retfd:%d\n",retfd);
		if(retfd<0){
				printf("select is failed\n");
			return -1;
		}

		/* Keypad event1  */
		if(FD_ISSET(fd2,&read_fileset)){
			printf("Keyboard event\n");
			ret = read(fd2,&ev,sizeof(struct input_event));
        		printf("ret:%d ev.type:%d ev.code:%d ev.value:%d\n",ret,ev.type,ev.code,ev.value);
		}

		/* Mouse data event0  */
				if(FD_ISSET(fd1,&read_fileset)){
					printf("data is available on mouse event0\n");
					ret = read(fd1,&ev,sizeof(struct input_event));
		        		printf("ret:%d ev.type:%d ev.code:%d ev.value:%d\n",ret,ev.type,ev.code,ev.value);
				}
	}
	return 0;
}


		
