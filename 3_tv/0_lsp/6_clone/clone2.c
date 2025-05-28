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

/* This will show you how to create a new process with clone and share data between processes*/

#define _GNU_SOURCE

#include <stdio.h>
#include <sys/mman.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>

#define STACK_SIZE  16000

int variable;
int dosomething(void *arg)
{
	printf("%s: In child process \n",__func__);
	variable = 40;
	return 0;
}

int main()
{
	void *child_stack;
	variable = 10;
	/*get memory for child stack */
	printf("%s: Value of variable now is %d\n",__func__, variable);
	
	child_stack = malloc(STACK_SIZE);
	if (child_stack == NULL) {
		perror("\nError creating \n");
		exit(1);
	}
	/*create new child with clone */
	clone(dosomething, child_stack + STACK_SIZE, CLONE_VM | CLONE_FILES ,NULL);
	/*sleep some time so that child can change 'variable' */
	sleep(2);
	printf("%s :Value of variable now is %d\n",__func__, variable);
}
