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
#include "mydata.h"
#include <semaphore.h>

int main()
{
	struct mydata *addr;		/* Pointer to shared memory region */
	pid_t cpid, cpid1;
	int i;

	/* setup and initialise shared data */
	addr =
	    (struct mydata *)mmap(NULL, sizeof(struct mydata),
				  PROT_READ | PROT_WRITE,
				  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED)
		perror("mmap");

	/* initialise unamed semaphore */
	sem_init(&(addr->mysem), 1, 1);
	addr->a = 0;
	addr->b = 0;
	
	/* start child 1 */
	cpid = fork();
	if (cpid == 0) {
		printf("CHILD 1: contending for data access\n");		
		sem_wait(&(addr->mysem));	/* decrement sem */
		printf("CHILD 1: committing shared data updates\n"); 
		addr->a = 20;
		sleep(10);
		addr->b = 40;
		printf("CHILD 1: releasing lock\n");
		sem_post(&(addr->mysem));	/* increment sem */
		if (munmap(addr, sizeof(int)) == -1)
			perror("munmap");
		exit(EXIT_SUCCESS);
	}

	else {
		/* start child 2 */
		cpid1 = fork();
		if (cpid1 == 0) {
			
			printf("CHILD 2: contending for data access\n");		
			sem_wait(&(addr->mysem));	/* decrement sem */
			
			printf("CHILD 2: lock acquired reading data values\n"); 
			printf("val in mydata.a	%d\n", addr->a);
			printf("val in mydata.b %d\n", addr->b);
			printf("CHILD 2: releasing lock\n"); 

			sem_post(&(addr->mysem));	/* increment sem */

			if (munmap(addr, sizeof(int)) == -1)
				perror("munmap");
			exit(EXIT_SUCCESS);

		} else {
			for (i = 0; i < 2; i++) 
				wait(NULL);

			if (munmap(addr, sizeof(int)) == -1)
				perror("munmap");
			exit(EXIT_SUCCESS);
		}
	}
}
