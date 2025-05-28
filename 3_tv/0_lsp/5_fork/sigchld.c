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
#include <signal.h>
#include <stdlib.h>

#define CHILD 0

int childstatus;

/* signal handler to handle SIGCHLD event */
void sighand(int signum)
{
	printf(" I am in sig handler %d\n",getpid());
	wait(NULL);
}

int main()
{
	pid_t cpid;
	cpid = fork();
	if (cpid == CHILD) {
		printf(" I am in child task %d \n", getpid());
		sleep(10);
		exit(100);
	}
	else {
		/* register signal handler with process manager */
		signal(SIGCHLD, sighand);

		while (1) 
		{
			printf(" in loop \n");
			sleep(2);
		}
	}
	return 0;
}

/* 
   void handler(int signum)
   {
	wait(NULL);
   }

   int main()
  {
   	cPid = fork();
   	if(cPid  == 0)
        	code that runs in child
   	else
	{
        	signal(SIGCHLD, handler);
        	code that runs in parent
	}
  }
*/
