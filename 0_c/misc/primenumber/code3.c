
/* sqrt method :
 *  N = a*b
 *
 *  1<=a<=b<=N;
 *
 *  a^2<N
 *
 *  N = 36
 *  a,b
 *  6 6
 *  4 9
 *
 *  a^2 < N
 *
 *  a < sqrt(N)
 *
 *  a and b are two numbers ;
 *
 *  a can be least possible value lessthan sqrt(N)
 *
 *  if we found any ONE factor < sqrt(N), then it is not prime;
 *
 */

/*
 * to avoid  undefined reference to `sqrt' error?
 * gcc code3.c -lm
 *
 */

#include <stdio.h>
#include <math.h>

int isnumberprime(int n)
{

   //for(int i=2;i<=n/2;i++)
   // for(int i=2;i<=sqrt(n);i++)
	for(int i=2; i*i<=n;i++)
   {
	 if(n%i == 0)
	 {
		 return 0;
	 }
   }

   return 1;
}


int main()
{
  int n =19;

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
 * time complexity : 0(sqrt(n);
 */
