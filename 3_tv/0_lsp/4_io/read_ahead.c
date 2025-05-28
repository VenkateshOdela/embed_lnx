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
#include <unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
	char buf[30];
	int fd, rev;

	if (fd = open("./linux.mp4", O_RDWR) < 0) {
		printf("\n open to fail");
		exit(1);
	}

/*	posix_fadvise(fd, 0 , 20, POSIX_FADV_NORMAL);	 Ok with default read_ahead */
	rev = posix_fadvise(fd, 0, 20, POSIX_FADV_SEQUENTIAL);	// Enable full read_ahead 
	if (rev != 0)
		perror("posix_fadvise : ");
/*	posix_fadvise(fd, 0 , 20, POSIX_FADV_RANDOM);  Disable read_ahead */

	if (read(fd, buf, 1000) < 0) {
		printf("\n read error");
		exit(1);
	}
	printf("\n The read  msg : %s", buf);
	close(fd);
	exit(EXIT_SUCCESS);

}
