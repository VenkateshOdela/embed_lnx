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

#include<stdio.h>
#include<sys/mman.h>
#include<sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdlib.h>
#define STACK_SIZE 16000

int fd;
int dosomething(void *arg)
{
//	sleep(2);
	printf("\nIn child process\n");
	/*close the file */
	close(fd);
}

int main()
{
	int ret;
	void *child_stack;
	char buf[100]={};


	fd=open("./test",O_RDWR);
	if(fd==-1){
		perror("");
		exit(1);
	}

	/*get memory for child stack*/
	child_stack=malloc(STACK_SIZE);
	if(child_stack==NULL){
		perror("\nError creating \n");
		exit(1);
	}
	/*create new child with clone*/
	clone(dosomething, child_stack+STACK_SIZE ,CLONE_VM |CLONE_FILES,NULL);
	
	sleep(2);
	
	ret=read(fd,buf,100);
	printf("%s: ret val%d\n",__func__,ret);
	if(ret<=0){
		perror("Read error");
		exit(1);
	}
	printf("%s: Data : %s\n",__func__, buf);
	wait(NULL);
}
