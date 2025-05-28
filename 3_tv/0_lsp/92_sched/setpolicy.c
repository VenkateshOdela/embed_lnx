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

void query_policy()
{
	struct sched_param param;
	int policy;
	policy = sched_getscheduler(0);
	sched_getparam(0, &param);

	switch (policy) {
	case SCHED_OTHER:
		printf("%s: current policy in force :SCHED_OTHER & prio: %d\n",
		       __func__, param.sched_priority);
		break;

	case SCHED_BATCH:
		printf("%s: current policy in force :SCHED_BATCH & prio: %d\n",
		       __func__, param.sched_priority);
		break;
	case SCHED_RR:
		printf("%s: current policy in force :SCHED_RR & prio %d\n",
		       __func__, param.sched_priority);
		break;
	case SCHED_FIFO:
		printf("%s: current policy in force :SCHED_FIFO & prio %d\n",
		       __func__, param.sched_priority);
		break;
	}
}

int main()
{
	struct sched_param param;
	query_policy();
	param.sched_priority = 30;
	sched_setscheduler(0, SCHED_RR, &param);
	getchar();
	query_policy();
	return 0;
}
