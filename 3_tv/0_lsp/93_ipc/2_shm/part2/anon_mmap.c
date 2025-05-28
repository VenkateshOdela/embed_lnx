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

#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main()
{
	int *addr;		/* Pointer to shared memory region */
	pid_t cpid;

	/* Parent creates mapped region prior to calling fork() */
	addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
		    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED)
		perror("mmap");

	*addr = 1;		/* Initialize integer in mapped region */

	cpid = fork();
	if (cpid < 0)
		perror("fork");

	if (cpid == 0) {	/* Child: increment shared integer and exit */
		printf("Child started, value = %d\n", *addr);
		(*addr)++;
		if (munmap(addr, sizeof(int)) == -1)
			perror("munmap");
		exit(EXIT_SUCCESS);
	}

	else {			/* Parent: wait for child to terminate */
		if (wait(NULL) == -1)
			perror("wait");
		printf("In parent, value = %d\n", *addr);
		if (munmap(addr, sizeof(int)) == -1)
			perror("munmap");
		exit(EXIT_SUCCESS);
	}
}
