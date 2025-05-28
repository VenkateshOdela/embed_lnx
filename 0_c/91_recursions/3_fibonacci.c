
/*
 *  0 , 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89
 *
 *
 */

#include <stdio.h>

int arr[20];

int fibanacci(int a)
{
   if((a < 0) || (a > 20))
   {
	   printf("invalid input \n");
	   return 0;
   }
   static int i =2;

   if(a == 0)
   {
	   return arr[i-2];
   }

   arr[i] = arr[i-1] + arr[i-2];
   i++;

   return fibanacci(a-1);

}

int main()
{
   int a=3;
   int ret;

   arr[0]=0;
   arr[1]=1;

   ret = fibanacci(a);

   printf("%d \n",ret);
}
