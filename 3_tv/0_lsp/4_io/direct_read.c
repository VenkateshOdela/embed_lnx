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

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>

int main()
{
	int fd, ret;
	ssize_t numRead;
	size_t size, alignment = 512;
	off_t offset;
	void *buf;

	size = getpagesize();
/*
   O_DIRECT: 
   configure user buffer (passed as agrument to read()) to be considered as 
   file cache, this buffer is passed to storage driver for direct transfer.
 
*/
	fd = open("./testfile", O_RDONLY | O_DIRECT);
	if (fd == -1) {
		perror("open");
		exit(1);
	}
   /* 
      set up sector aligned buffer, only such allocations are suitable for
      file cache.
   */
	ret = posix_memalign(&buf, alignment, size * 8);
	if (ret < 0) {
		perror("memalign");
		exit(1);
	}

    /* initiate read() on the buffer for direct transfer */
	numRead = read(fd, buf, size);
	if (numRead == -1) {
		perror("read");
		exit(1);
	}
	printf("Read %ld bytes\n", (long)numRead);

}
