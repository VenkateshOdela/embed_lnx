#include<stdio.h>
#include<string.h>

int cmpr_strings(char *strng_ptr);

int main()
{
  char string_1[] = "C000";
  
  cmpr_strings(string_1);
  
  
  
}

int cmpr_strings(char *strng_ptr)
{
  printf("%s\n",strng_ptr);
  char string_2[] = "C000";

  int ret_val;
  ret_val = strcmp(strng_ptr,string_2);

  printf("%d \n",ret_val);
}
