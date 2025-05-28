/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * ******************************************************************************
 * This program is part of the source code provided with "Linux Systems Programming" Course
 * (c) 2019- 2021 Techveda, Hyderabad, India
 * website : www.techveda.org
 * Author/Trainer:  Raghu Bharadwaj
 * ******************************************************************************
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 */

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int fd;
	void *filep;
	
	/* create new file and fetch file desctiptor to it */
	fd = open("./mapfile",O_CREAT | O_RDWR, S_IRWXU);
	if(fd < 0){
		perror("open: \n");
		exit(1);
	}

	printf("pid %d\n",getpid());
	
	/* reserve disk blocks for new file */
	posix_fallocate(fd, 0, 8192);

	getchar();
	
	/*  map file cache to mmap segment of the process space */
	filep = mmap(0, 8192, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(filep == MAP_FAILED){
		printf("mapping failed\n");
		exit(2);
	}
	close(fd);
	/* access file cache through mapped pointer */
	memset(filep, 'm', 8192);

	getchar();	
	/* unmap file from the address space */
	munmap(filep, 8192);

	getchar();
	return 0;
}

