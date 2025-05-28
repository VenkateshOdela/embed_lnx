/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * ******************************************************************************
 * This program is part of the source code provided with
 * "Linux Systems Programming Essentials" Course offered by TECHVEDA(c) 2020 - 2022
 * Course page : https://www.techveda.org/linuxsysprg
 * Trainer:  Raghu Bharadwaj
 * ******************************************************************************
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE  512
#define MAX 3

/* Function to fill the iovec values */
void populate_iovec(struct iovec *iovec, char *ptr, int size)
{
	iovec->iov_base = ptr;
	iovec->iov_len = size;
}

int main()
{
	int fd, rbytes;
	char read_buf1[BUFFER_SIZE];
	char read_buf2[BUFFER_SIZE];
	char read_buf3[BUFFER_SIZE];
	char read_buf4[BUFFER_SIZE];
	struct iovec iovec[4];

	fd = open("./linux.mp4", O_RDONLY);

	populate_iovec(iovec, read_buf1, sizeof(read_buf1));
	populate_iovec(iovec + 1, read_buf2, sizeof(read_buf2));
	populate_iovec(iovec + 2, read_buf3, sizeof(read_buf3));
	populate_iovec(iovec + 3, read_buf4, sizeof(read_buf4));

	/* The  readv()  function  reads  iovcnt  buffers  from  the file 
	 *  associated with the file descriptor fd into the buffers
	 *  iov buffers 
	 *  file associated with the  file  descriptor
	 */
	rbytes = readv(fd, iovec, 4);

	printf("\n Total number of bytes read from file : %d \n", rbytes);
	printf(" %s", read_buf1);
	printf(" %s", read_buf2);
	printf(" %s", read_buf3);
	printf(" %s\n", read_buf4);

	close(fd);
}
