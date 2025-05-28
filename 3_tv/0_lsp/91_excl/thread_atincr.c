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
#include <string.h>

static int glob = 0;

static void *tStart(void *arg)
{
	int local, j;
	int loops = *((int *)arg);
	for (j = 0; j < loops; j++) {
		__sync_fetch_and_add(&glob, 1); /* gnuc atomic op macro */
	}
	return NULL;
}

int main()
{
	pthread_t t1, t2;
	int loops = 10000000, ret;

	ret = pthread_create(&t1, NULL, tStart, &loops);
	if (ret != 0)
		perror("Pthread Create: ");

	ret = pthread_create(&t2, NULL, tStart, &loops);
	if (ret != 0)
		perror("Pthread Create:  ");

	ret = pthread_join(t1, NULL);
        if (ret != 0)
                printf("%s: Pthread Join failed:%s\n", __func__, strerror(ret));

        ret = pthread_join(t2, NULL);
        if (ret != 0)
                printf("%s: Pthread Join failed: %s\n", __func__,
                       strerror(ret));

        printf("%s: glob = %d\n", __func__, glob);
        return 0;
}
