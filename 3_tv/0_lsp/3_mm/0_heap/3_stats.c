/*
 *  example of heap expanding more than 135 K
 *  heap size 135k;
 *
 *
 *   ./a.out 10
 *
 *     - 10*1024 = 10K
 *
 *     - 150*10K =
 *
 *   1. setup 15OK block
 *   2. free 70K
 *   3. Allocating 140K
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define NUM_CHUNKS 150

int main(int argc, char *argv[])
{
	void *p[NUM_CHUNKS], *p1;
	int size, count = 0;
	if (argc != 2) {
		fprintf(stderr, "usage :./a.out <num bytes to allocate> \n");
		exit(1);
	}

	size = atoi(argv[1]);

	for (count = 0; count <= NUM_CHUNKS; count++) {
		p[count] = malloc(size * 1024);
		if (!p[count]) {
			fprintf(stderr, "malloc failed\n");
			exit(1);
		}
		printf("allocated @ %p\n", p[count]);
		malloc_stats();
		getchar();
	}

	for (count = 0; count <= 70; count++) {
		free(p[count]);
		malloc_stats();
		getchar();
	}

	printf("Attempting large(>135k)  allocation\n");
	getchar();
	p1 = malloc(140 * 1024);
	if (!p1) {
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}

	malloc_stats();
	getchar();

	return 0;
}
