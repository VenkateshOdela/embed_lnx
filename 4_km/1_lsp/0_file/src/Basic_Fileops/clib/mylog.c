/*
 * 1. This program helps in logging status of code
 */


#include <stdio.h>
#include <time.h>

FILE *myfp=NULL;

void init_mylog()
{
	myfp = fopen("mylog", "a");
	if (myfp == NULL)
	{
		printf("\n /var/mylog.fwex: OPEN ERROR");
	}
	else
	{
		fprintf(myfp, "mylog file created \n");fflush(myfp);

	}
}

int main()
{
	time_t t;
	printf ("current time: %ld\n", (long) time (&t));
	printf ("the same value: %ld\n", (long)t);
	init_mylog();
	fprintf(myfp, "venkatesh odela \n");fflush(myfp);
	return 0;
}

/*
 * fopen modes :
 *
       r      Open text file for reading.
       r+     Open for reading and writing.
       w      Truncate file to zero length or create text file for writing.
       w+     Open for reading and writing.  The file is created if it does not exist, otherwise it is truncated.
       a      Open  for  appending (writing at end of file).  The file is created if it does not exist.
              The stream is positioned at the end of the file.
       a+     Open for reading and appending (writing at end of file).  The file is created if it does not exist.
              The initial file position for reading is at the beginning of the file, but
              output is always appended to the end of the file.
 */

/*
 * 1.extern FILE *myfp; is used in other .c files
 *   and fprintf(myfp,"text") can be used in other files;
 *
 */
