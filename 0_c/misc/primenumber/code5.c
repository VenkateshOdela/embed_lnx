/*
 * print prime numbers between 1 to N
 */


#include <stdio.h>

int main()
{
  int n =100;
  int cnt=0;

  int arr[n];
  arr[0]=0;
  arr[1]=0;

  for(int i =2; i<=n; i++)
  {
	  arr[i] = 1;
  }

  for( int i=2; i*i<=n;i++)
  {
	  if(arr[i] == 1)
	  {
		  for(int j = i*i; j<=n;j= j+i)
		  {
			  if(arr[j] == 1)
			  {
				  if(j%i == 0)
				  {
					  arr[j] =0;
				  }
			  }
		  }
	  }
  }

  for(int i = 2; i<=n; i++)
  {
	  if(arr[i]==1)
	  {
		  printf("%d ,", i);
	  }

  }



}

/*
The Sieve of Eratosthenes is an ancient algorithm for finding all the prime numbers up to a given limit n.
The algorithm starts by creating a list of integers from 2 to n, and then iteratively marking off all the
multiples of each prime number in the list, starting with 2.
The unmarked numbers that remain after all the multiples have been marked are prime.

Here's how the algorithm works in more detail:

-Create a list of all the integers from 2 to n.
-Starting with the first prime number 2, mark off all its multiples in the list.
-Move to the next unmarked number in the list (which is the next prime number), and mark off all its multiples in the list.
-Repeat step 3 until you have processed all the numbers up to the square root of n.
-The unmarked numbers that remain in the list are prime.
-For example, to find all the prime numbers up to 30 using the Sieve of Eratosthenes:

*/
