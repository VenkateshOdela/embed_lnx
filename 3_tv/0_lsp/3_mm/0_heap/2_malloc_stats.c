/*
 * malloc_stats() function gives how usage of heap and mmap bytes cureently in use.
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

int main(int argc, char *argv[])
{
	void *p;
	int size;
	if(argc != 2){
		fprintf(stderr, "usage :./a.out <num bytes to allocate> \n");
		exit(1);
	}

	size = atoi(argv[1]);
	/* Allocate heap block and observe allocation stats */
	p = malloc(size * 1024);
	if (!p){
		fprintf(stderr,"malloc failed\n");
		exit(1);
	}
	
	printf("allocated @ %p\n", p);
	getchar();
	malloc_stats();

	/* free allocated chunk and observe allocation stats */
	free(p);
	getchar();
	malloc_stats();
	return 0;
}
