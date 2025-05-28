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

#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void query_policy(pid_t pid)
{
	struct sched_param prio;
	int policy, ret;
	policy = sched_getscheduler(pid);
	sched_getparam(0, &prio);
	if(policy & SCHED_RESET_ON_FORK){
			printf("Fork reset enabled\n");
			policy= policy & ~(SCHED_RESET_ON_FORK);
			}
	switch (policy) {
	case SCHED_OTHER:
		printf
		    ("%s: caller pid :%d sched policy :SCHED_OTHER sched prio: %d\n",
		     __func__, getpid(), prio.sched_priority);
		break;
	case SCHED_RR:
		printf
		    ("%s: caller pid :%d & sched policy :SCHED_RR sched prio: %d\n",
		     __func__, getpid(), prio.sched_priority);
		break;
	case SCHED_FIFO:
		printf
		    ("%s: caller pid :%d & sched policy :SCHED_FIFO sched prio : %d\n",
		     __func__, getpid(), prio.sched_priority);
		break;
	default:
		printf("Error\n");
		break;
	}
}

int main()
{
	pid_t cpid;
	struct sched_param param;
	param.sched_priority = 60;
	
	query_policy(getpid());
	getchar();

	sched_setscheduler(0, SCHED_RR /* | SCHED_RESET_ON_FORK */, &param);
	query_policy(getpid());
	getchar();

	printf("%s: forking child\n", __func__);
	cpid = fork();
	if (cpid == 0) {
		printf("%s: in child process with pid %d\n", __func__,getpid());
		query_policy(getpid());
		exit(0);
	} 
	else {
		wait(NULL);
		query_policy(getpid());
		printf("%s: end of parent \n", __func__);
	}
	return 0;
}
