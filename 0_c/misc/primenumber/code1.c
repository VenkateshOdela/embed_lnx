
/* two loops method; checking count 2 */

/* print numbers between 1 to 10*/
/* 2,3,5,7*/
/*
 *  prime number
 *   - a number divisable by one;
 *   - a number divisable by itself;
 */

/*
 * Disadvantage :
 * time complexity is more n^2
 *
 */

#include <stdio.h>

int main()
{
	int cnt=0;
	for(int N=1;N<=100;N++)
	{
		cnt =0;
		for(int i=1; i<=N;i++)
		{
			if(N%i == 0)
			{
				cnt++;
			}
		}

		if(cnt == 2)
		{
			printf("%d ",N);
		}
	}
}

/* 1-10
 *  2 3 5 7
 *
 * 1 -20
 * 2 3 5 7 11 13 17 19
 */

/*
 *  time complexity : 0(n2);
 */
