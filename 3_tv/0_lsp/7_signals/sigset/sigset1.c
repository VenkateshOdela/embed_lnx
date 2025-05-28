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
#include <signal.h>

void sighand(int no)
{
	printf(" I am in sighandler %d \n", no);
}

int main()
{
	sigset_t s_set;
	signal(62, sighand);
	signal(4, sighand);
	signal(34, sighand);
	signal(2, sighand);


	printf("pid %d\n", getpid());
	sigemptyset(&s_set);
	sigaddset(&s_set, 62);
	sigaddset(&s_set, 4);
	sigaddset(&s_set, 34);
	sigaddset(&s_set, 2);

	sigprocmask(SIG_BLOCK | SIG_SETMASK, &s_set, NULL);

	/* code runs with signals specified in mask disabled */
	printf(" %s: Send me signal one and see the effect now \n",__func__);
	getchar();

	sigprocmask(SIG_UNBLOCK, &s_set, NULL);
	printf(" %s:signals are unblocked \n",__func__);
//	while (1);
	return 0;// should never reach here
}
