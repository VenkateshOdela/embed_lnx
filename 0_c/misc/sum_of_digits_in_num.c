#include <stdio.h>

int main()
{
  int number =1234;
  int sum = 0;
  while(number>0)
 {
   sum += number%10;
   number =number/10;
 }

 printf("sum 0f digits : %d \n",sum);
}
