/*
 * 1. This programm will print out all the kernel ring buffer contents
 *
 * ******************************************************************************
 * syslog, klogctl - read and/or clear kernel message ring buffer; set console_loglevel
 *
 *      int syslog(int type, char *bufp, int len);
       // No wrapper provided in glibc

       // The glibc interface
       #include <sys/klog.h>

       int klogctl(int type, char *bufp, int len)
 */

#include <sys/klog.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
	int len, level;
	char *buf;
	int last = '\n';
	int in = 0;

	len = klogctl(10, NULL, 0);	/* read ring buffer size */

	if (len < 16 * 1024)
		len = 16 * 1024;

	if (len > 16 * 1024 * 1024)
		len = 16 * 1024 * 1024;

	buf = malloc(len);
	if (!buf) {
		printf("\nCannot allocate memory\n");
		return 0;
	}

	len = klogctl(3, buf, len);	/* read ring buffer */
	if (len < 0) {
		printf("\nCannot Read the buffer\n");
		free(buf);
		return 0;
	}
	if (len == 0)
		return 0;


    /* printing through console */
	/* Skip <#> at the start of lines */
	while (1) {
		if (last == '\n' && buf[in] == '<') {
			in += 3;
			if (in >= len)
				break;
		}
		last = buf[in];
		putchar(last);
		in++;
		if (in >= len)
			break;
	}
	/* Make sure we end with a newline */
	if (last != '\n')
		putchar('\n');

	free(buf);

	return 0;
}

/*
 *  int klogctl(int type, char *bufp, int len);
 *
 *  The  type  argument  determines the action taken by this function.
 *
 *         SYSLOG_ACTION_CLOSE (0)
              Close the log.  Currently a NOP.

             SYSLOG_ACTION_READ_ALL (3)
              Read  all  messages  remaining in the ring buffer, placing them in the buffer pointed to by bufp.  The call reads the last len
              bytes from the log buffer (nondestructively), but will not read more than was written into the buffer since  the  last  "clear
              ring buffer" command (see command 5 below)).  The call returns the number of bytes read.



       SYSLOG_ACTION_SIZE_BUFFER (10) (since Linux 2.6.6)
              This command returns the total size of the kernel log buffer.  The bufp and len arguments are ignored.
 *
 */
