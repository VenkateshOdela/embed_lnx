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
#include <string.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE  100
#define MAX 3

/* Function to fill the iovec values */
void populate_iovec(struct iovec *iovec, char *ptr, int size)
{
	iovec->iov_base = ptr;
	iovec->iov_len = size;
}

int main()
{
	int fd, wbytes;
	char write_buf1[BUFFER_SIZE] =
	    " The writve() function shall be equivalent to write(). ";
	char write_buf2[BUFFER_SIZE] =
	    " The writev() function writes iovcnt buffers of data described ";
	char write_buf3[BUFFER_SIZE] =
	    " by iov to the file associated with the ";
	char write_buf4[BUFFER_SIZE] = " file  descriptorfd.\n ";

	struct iovec iovec[4];

	/* Open Write.txt file for writing */
	fd = open("Write.txt", O_CREAT | O_WRONLY, 0644);

	populate_iovec(iovec, write_buf1, strlen(write_buf1));
	populate_iovec(iovec + 1, write_buf2, strlen(write_buf2));
	populate_iovec(iovec + 2, write_buf3, strlen(write_buf3));
	populate_iovec(iovec + 3, write_buf4, strlen(write_buf4));

	/* The writev() function writes iovcnt buffers of data to the 
	 *  file associated with the  file  descriptor
	 */
	wbytes = writev(fd, iovec, sizeof(iovec) / sizeof(struct iovec));
	printf("\n Total number of bytes written inti file : %d \n", wbytes);
	close(fd);
}
