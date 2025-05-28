/*
 *
		-	mknod - make block or character special files
		- 	   b      create a block (buffered) special file
			   c, u   create a character (unbuffered) special file
			   p      create a FIFO

			-	sudo mknod /dev/abc c 190 0
				-	c means char device
				-	190 major number
				-	0 means minor number
 *
 *
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
	int fd, i;
	ssize_t ret;
	char my_buf[4096]="Hello world";

	fd = open( "/dev/abc", O_RDWR );
	if(fd < 0) {
		printf("failed acquiring file descriptor return status %d\n",fd);
		return fd;
	}

	getchar();	
	/* write the contents of my buffer into the device */
	ret = write( fd, my_buf, 12 );

	getchar();
	ret = read(fd,my_buf,3000);
	if(ret<0)
		printf("read operation failed with return status %ld\n",ret);
	getchar();
	close(fd);
}
	
