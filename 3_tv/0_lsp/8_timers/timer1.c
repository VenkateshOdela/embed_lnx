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
#include <signal.h>
#include <sys/time.h>
#include <time.h>

static volatile sig_atomic_t gotAlarm = 0;

static void sigalrmHandler(int sig)
{
    printf(" got sigalarm %d times \n", ++gotAlarm);
}

int main()
{
    struct itimerval itv,itv1;
    struct sigaction sa;


    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigalrmHandler;
    if (sigaction(SIGALRM, &sa, NULL) == -1)
        perror("sigaction");

    /* Set timer */

    itv.it_value.tv_sec = 1;  // initial event
    itv.it_value.tv_usec = 0;
    itv.it_interval.tv_sec = 0;// periodic interval 
    itv.it_interval.tv_usec = 500;


    if (setitimer(ITIMER_REAL, &itv, NULL) == -1){
        perror("setitimer : ");
	exit(1);
	}


    itv1.it_value.tv_sec = 3;  // initial event
    itv1.it_value.tv_usec = 0;
    itv1.it_interval.tv_sec = 3;// periodic interval 
    itv1.it_interval.tv_usec = 0;


    if (setitimer(ITIMER_REAL, &itv1, NULL) == -1){
        perror("setitimer : ");
	exit(1);
    }

   while(1)
   {
	   printf("busy loop\n");
	   sleep(1);
   }

   return 0; 

}
