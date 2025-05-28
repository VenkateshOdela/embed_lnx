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
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define DEATH(mess) { perror(mess); exit(errno); }

/* our signal handlers */

void handler(int what)
{
	printf("%s: received SIGINT Executing work\n", __func__);
	sleep(5);
	printf("%s: exit from handler\n",__func__);
}

int main()
{
	struct sigaction act;
	sigset_t sigmask;

	memset(&act, 0, sizeof(act));

	sigemptyset(&sigmask);
	sigaddset(&sigmask, SIGQUIT);
	sigaddset(&sigmask, SIGTERM);
	sigaddset(&sigmask, SIGSEGV);

	/* Install signal handlers */
	act.sa_handler = handler;
	act.sa_mask = sigmask;	

	if (sigaction(SIGINT, &act, NULL) < 0)
	{
		perror("sigaction \n");
		exit(1);
	}
	/* Do something pointless, forever */
	for (;;) {
		printf("%s: This is a pointless message.\n",__func__);
		sleep(1);
	}

	return 0;
}
