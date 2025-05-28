/*
 *
 *
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define FILEPATH "/sys/kernel/debug/dbgvtrans/address"

int main(){

	int fd = 0;
	FILE *fp;
	
	int v=0;
	int val = 10;

	printf("Translating Virtual Addess %p\n", (void *)&v);
	fd = open(FILEPATH, O_WRONLY);
	if (fd < 0)
		printf("open failed\n");

	fp = fdopen(fd, "w");
	if (fp == NULL)
		printf("fdopen() failed\n");

	fprintf(fp,"%p:%d",(void *)&v,++val);
	fclose(fp);

	getchar();
	printf("value in v = %d\n",v);
	return 0;
}
