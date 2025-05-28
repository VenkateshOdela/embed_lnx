#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define DEV_PATH    "/dev/vDev-1"
#define BUFF_SIZE   512

char stringSend[BUFF_SIZE];
char receive[BUFF_SIZE];

int main()
{

	int fd, ret;
	char option;

	printf("Initiating device/driver test...\n");
	fd = open(DEV_PATH, O_RDWR);
	if (fd < 0) {
		printf("Failed to open the device\n");
		return errno;
	}

	printf("Type in a short string:\n");
	scanf(" %[^\n]%*c", stringSend);

	ret = write(fd, stringSend, BUFF_SIZE);
	if (ret < 0) {
		printf("Failed to write the message to the device\n");
		return errno;
	}

	lseek(fd, 0, SEEK_SET);
	printf("ENTER to read back from the device...\n");
	getchar();

	ret = read(fd, receive, BUFF_SIZE);
	if (ret < 0) {
		printf("Failed to read the message from the device\n");
		return errno;
	}
	printf("The received message is: %s\n\n", receive);
	return 0;
}
