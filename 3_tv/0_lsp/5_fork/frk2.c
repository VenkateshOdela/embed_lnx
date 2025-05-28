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

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

#define CHILD 0

int main()
{
	pid_t cPid;
	int childstatus;

	cPid = fork();
	if (cPid == CHILD) {
		printf("%s:In child process %d\n", __func__, getpid());
		getchar();
		exit(100);
	} else {
		wait(&childstatus);
		if (WIFEXITED(childstatus)) {
			printf("exited, status=%d\n", WEXITSTATUS(childstatus));
		} else if (WIFSIGNALED(childstatus)) {
			printf("killed by signal %d\n",
			       WIFSIGNALED(childstatus));
		} else if (WIFSTOPPED(childstatus)) {
			printf("stopped by signal %d\n", WSTOPSIG(childstatus));
		} else if (WIFCONTINUED(childstatus)) {
			printf("continued\n");
		}
	}
	return 0;
}

/* 
   cPid = fork();
   if(cPid  == 0)
	code that runs in child
   else
	wait(NULL); // synchronous destruction 
	code that runs in parent

wait(int *childstatus)
{
	step 1: suspend caller until child terminates
	step 2: gather exit value of child and return that as out param
		(Cause of child termination can be discovered through 
		 this value)
	step 3: instruct process manager to destroy defunct child
}
*/
