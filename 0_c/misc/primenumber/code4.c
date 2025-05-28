
/* print prime numbers between 1 to N */

#include <stdio.h>

int main()
{
	int flag=0;
	for(int N=1;N<=100;N++)
	{
	   flag=0;
       for(int i=2;i*i<=N;i++)
       {
    	   if(N%i == 0)
    	   {
    		   flag =1;
    		   break;
    	   }
       }

       if(flag == 0 && N!=1)
       {
    	   printf("%d ",N);
       }
	}
}
