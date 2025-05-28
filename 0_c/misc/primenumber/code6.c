
/* N/2 Method :
 * print prime numbers between 1 to 10
 *
 *
 * */


#include <stdio.h>


int main()
{
   int flag =0;

	for(int N=1; N<10; N++)
	{
        flag =0;
		for(int i=2; i<= N/2;i++)
		{
			if(N%i == 0)
			{
				flag =1;
				break;
			}

		}

		if(flag == 0)
		{
			printf("%d",N);
		}


	}


}


