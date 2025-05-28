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
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

void handler(int signum)
{
	printf("%s:handler invoked for signal : %d\n", __func__, signum);
	printf("%s: exit from sighandler\n",__func__);
}

int main()
{
	struct sigaction act;
	int fd, ret;
	char buf[20];

	memset(&act, 0, sizeof(act));
        act.sa_flags = SA_RESTART;
        act.sa_handler = handler;

	if (sigaction(SIGINT, &act, NULL) < 0)	
	{
		perror("sigaction \n");
		exit(1);
	}

	fd = open("./myfifo", O_RDWR);
	if (fd < 0)
	{
		perror("open \n");
		return fd;
	}
	printf("%s:Descriptor created with fd:%d\n",__func__, fd);
	ret = read(fd, buf, 30);
	if (ret < 0){
		perror("read:");
		return ret;
	}
	printf("%s: no of bytes read %d :Data read %s \n",__func__, ret, buf);
	return 0;
}
