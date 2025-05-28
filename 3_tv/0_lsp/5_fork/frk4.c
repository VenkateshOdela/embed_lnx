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
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

static int func(int arg)
{
	int j;

	for (j = 0; j < 0x100; j++)
		if (malloc(0x8000) == NULL) {
			perror("malloc");
			exit(1);
		}
	printf("Program break in child:  %10p\n", sbrk(0));
	malloc_stats();
	getchar();

	return arg;
}

int main(void)
{
	pid_t CPid;
	int status, arg;

	printf("Program break in parent: %10p\n", sbrk(0));
	malloc_stats();
	getchar();

	switch (fork()) {

	case -1:
		perror("fork");
		exit(1);

	case 0:
		/* Child calls func() and uses return value as exit 
		   status */
		exit(func(arg));

	default:

		/* Parent waits for child to terminate. It can determine the
		   result of func() by inspecting 'status' */

		if (wait(&status) == -1) {
			perror("wait");
			exit(1);
		}
		malloc_stats();
		getchar();
		printf("Program break in parent: %10p\n", sbrk(0));
	}
	return 0;
}
