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
#include <errno.h>
#include <signal.h>
#include <time.h>

void timestamp(char *str)
{
	time_t t;
	time(&t);
	printf("The time %s is %s\n", str, ctime(&t));
}

//char *buf = "hello";
int main()
{
	int result = 0;
	sigset_t waitset;
	siginfo_t info;
	pid_t cpid, ppid;
	union sigval val;

	/* let's disable async handlers and enable signal queue */
	sigemptyset(&waitset);
	sigaddset(&waitset, SIGRTMIN);
	sigprocmask(SIG_BLOCK, &waitset, NULL);

	cpid = fork();
	if (cpid != 0) {
		timestamp("before sigwaitinfo()");
		/* wait for signal to arrive */
		result = sigwaitinfo(&waitset, &info);
		if (result < 0)
			printf("sigwaitinfo failed : \n");
		/* got signal */
		printf
		    ("sigwaitinfo() returned for signal %d, with message %s\n",
		     info.si_signo, (char *)info.si_ptr);
		timestamp("after sigwaitinfo()");
		exit(0);
	} else {
		char *buf = "hello";
		ppid = getppid();
		val.sival_ptr = buf;
		sigqueue(ppid, SIGRTMIN, val);
		while (1) ;
	}
	return 0;
}
