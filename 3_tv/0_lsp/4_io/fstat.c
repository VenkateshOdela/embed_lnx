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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

static void displayStatInfo(const struct stat *sb)
{
	printf("File type:                ");

	switch (sb->st_mode & S_IFMT) {
	case S_IFREG:
		printf("regular file\n");
		break;
	case S_IFDIR:
		printf("directory\n");
		break;
	case S_IFCHR:
		printf("character device\n");
		break;
	case S_IFBLK:
		printf("block device\n");
		break;
	case S_IFLNK:
		printf("symbolic (soft) link\n");
		break;
	case S_IFIFO:
		printf("FIFO or pipe\n");
		break;
	case S_IFSOCK:
		printf("socket\n");
		break;
	default:
		printf("unknown file type?\n");
		break;
	}

	printf("I-node number:            %ld\n", (long)sb->st_ino);

	printf("Number of (hard) links:   %ld\n", (long)sb->st_nlink);

	printf("Ownership:                UID=%ld   GID=%ld\n",
	       (long)sb->st_uid, (long)sb->st_gid);

	if (S_ISCHR(sb->st_mode) || S_ISBLK(sb->st_mode))
		printf("Device number (st_rdev):  major=%ld; minor=%ld\n",
		       (long)major(sb->st_rdev), (long)minor(sb->st_rdev));

	printf("File size:                %lld bytes\n",
	       (long long)sb->st_size);
	printf("Optimal I/O block size:   %ld bytes\n", (long)sb->st_blksize);
	printf("512B blocks allocated:    %lld\n", (long long)sb->st_blocks);

	printf("Last file access:         %s", ctime(&sb->st_atime));
	printf("Last file modification:   %s", ctime(&sb->st_mtime));
	printf("Last status change:       %s", ctime(&sb->st_ctime));
}

int main(void)
{
	struct stat sb;

	if (stat("./linux.mp4", &sb) == -1) {
		perror("stat");
		exit(1);
	}

	displayStatInfo(&sb);

	exit(EXIT_SUCCESS);
}
