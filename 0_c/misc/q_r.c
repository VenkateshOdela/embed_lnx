#include <stdio.h>


int main()
{
 
  int number=4;
  int divisor=10;
  
  /*
     10)12345(1  -> is quotient
        10
        --
         234   -> remainder
  */
 
  
  int quotient = number/divisor;
  int remainder = number%divisor;


  printf("quotient = %d \n",quotient);
  printf("remainder = %d \n",remainder);
 

}
