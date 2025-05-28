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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct __Shdata_t{
	int a;
	int b;
	int c;
}Shdata_t;

Shdata_t glob; /* shared data object */
	
static void * writer(void *arg) /* Thread 1 */
{
    	glob.a=10;
    	sleep(1);
    	glob.b=20;
	sleep(1);
	glob.c = 30;
    	return NULL;
}

static void * reader(void *arg)		/* Thread 2 */
{
      printf("%s: glob.a = %d\n", __func__, glob.a);
      sleep(1);
      printf("%s: glob.b = %d\n", __func__, glob.b);
      sleep(1);
      printf("%s: glob.c = %d\n", __func__, glob.c);
      return NULL;
}

int main()
{

    pthread_t t1, t2;
    int ret;

    ret = pthread_create(&t1, NULL, writer, NULL);
    ret = pthread_create(&t2, NULL, reader, NULL);

    pthread_exit(NULL);

}
