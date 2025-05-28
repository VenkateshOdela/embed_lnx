
/* Find delay between two instances diff/dealy = t2-t1 */

/*
gettimeofday - get time

 int gettimeofday(struct timeval *tv, struct timezone *tz);

 struct timeval
 {
    time_t      tv_sec;     //seconds
     suseconds_t tv_usec;   // microseconds
   };
   above structure gives the number of seconds and microseconds
   since the Epoch

   The use of the timezone structure is obsolete;
   the tz argument should normally be specified as NULL.


*/


#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main()
{

	struct timeval t1,t2;
	unsigned long long diff, stMillis, edMillis;

	int count=0;
	while(count<10)
	{
		gettimeofday(&t1, NULL);
		sleep(1);
		sleep(1);
		gettimeofday(&t2, NULL);

		stMillis = (t1.tv_sec * 1000) + (t1.tv_usec / 1000);
		edMillis = (t2.tv_sec * 1000) + (t2.tv_usec / 1000);

		diff = edMillis - stMillis;

		//printf("diff(t2-t1) = %llums stMillis(t1) = %llu edMillis(t2) = %llu \n",diff,stMillis,edMillis);

		printf("diff /delay(t2-t1) = %llums \n",diff);
		count++;

	}

	return 0;
}


