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
#include <sys/signalfd.h>

#define DEATH(mess) { perror(mess); exit(errno); }

void timestamp(char *str)
{
	time_t t;
	time(&t);
	printf("The time %s is %s\n", str, ctime(&t));
}

int main(int argc, char *argv[])
{

	int result = 0;
	int sfd, j;
	ssize_t s;
	sigset_t waitset;
	struct signalfd_siginfo info;

	sigemptyset(&waitset);
	sigaddset(&waitset, SIGALRM);

	sigprocmask(SIG_BLOCK, &waitset, NULL);

	sfd = signalfd(-1, &waitset, 0);
	if (sfd == -1)
		DEATH("signalfd");

	alarm(5);

	timestamp("before read() on signalfd");

	s = read(sfd, &info, sizeof(struct signalfd_siginfo));
	if (s != sizeof(struct signalfd_siginfo))
		DEATH("read");

	printf("%s: got signal %d \n", argv[0], info.ssi_signo);
	if (info.ssi_code == SI_QUEUE) {
		printf("; ssi_pid = %d; ", info.ssi_pid);
		printf("ssi_int = %d", info.ssi_int);
	}

	timestamp("after read() on signalfd");

	return (result);
}
