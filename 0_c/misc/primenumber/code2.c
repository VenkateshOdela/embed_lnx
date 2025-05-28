
/* is number a prime : N/2 method */

/*
 * 20 factors
 * 2 4 5 10
 *
 * 30 factors
 * 2 5 6 10 15
 *
 * factors of number n  are found in lessthan n/2;
 *
 * 100 factors
 *
 * 2 4 5 10 20 25 50
 *
 *
 * Disadvantage as N increases the iterations also upto N/2 times;
 *  this is also large;
 *
 *  for given number N if we found any factor less than N/2 numbers,
 *  then it is not a prime number;
 *
 *
 */
int isnumberprime(int n)
{

   for(int i=2;i<=n/2;i++)
   {
	 if(n%i == 0)
	 {
		 return 0;
	 }
   }

   return 1;
}

#include <stdio.h>

int main()
{
  int n =14;

  int ret = isnumberprime(n);

 if(ret)
 {
	 printf("prime");
 }
 else{
	 printf("not prime");
 }
}

/*
 * time complexity : 0(n/2)
 */
