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
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void)
{
	int fd, flags, ret;

	char template[] = "/tmp/testXXXXXX";

	fd = mkstemp(template);
	if (fd == -1) {
		perror("mkstemp");
		exit(1);
	}

	switch (fork()) {

	case -1:
		perror("fork");
		exit(1);

	case 0:
		/* Child: change file offset and status flags */
		if (lseek(fd, 1000, SEEK_SET) == -1) {
			perror("lseek");
			exit(1);
		}

		flags = fcntl(fd, F_GETFL);	/* Fetch current flags */
		if (flags == -1) {
			perror("fcntl - F_GETFL");
			exit(1);
		}
		flags |= O_APPEND;	/* Turn O_APPEND on */
		if (fcntl(fd, F_SETFL, flags) == -1) {
			perror("fcntl - F_SETFL");
			exit(1);
		}
		break;

	default:

		/* lets test if Parent can see file-Descriptor changes made by child */
		wait(NULL);
		printf("Child has exited\n");
		printf("File offset in parent: %lld\n",
		       (long long)lseek(fd, 0, SEEK_CUR));
		flags = fcntl(fd, F_GETFL);
		printf("O_APPEND flag in parent is: %s\n",
		       (flags & O_APPEND) ? "on" : "off");
		break;
	}
	return 0;
}
