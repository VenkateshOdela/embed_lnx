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
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	ssize_t numRead;
	char buf[BUF_SIZE];

	if (argc != 3 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "%s old-file new-file\n", argv[0]);
		exit(1);
	}

	/* Open input and output files */

	inputFd = open(argv[1], O_RDONLY);
	if (inputFd == -1) {
		fprintf(stderr, "error opening source file");
		exit(1);
	}

	openFlags = O_CREAT | O_WRONLY | O_SYNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;	/* rw-rw-rw- */

	outputFd = open(argv[2], openFlags, filePerms);
	if (outputFd == -1) {
		fprintf(stderr, "error opening source file");
		exit(1);
	}

	/* Transfer data until we encounter end of input or an error */

	while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
		if (write(outputFd, buf, numRead) != numRead) {
			perror("write ");
			exit(1);
		}
	if (numRead == -1) {
		perror("read: ");
		exit(1);
	}

	close(inputFd);
	close(outputFd);

	exit(EXIT_SUCCESS);
}
