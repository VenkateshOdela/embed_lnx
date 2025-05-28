#include<stdio.h>
#include<string.h>

#define FALSE 0
#define TRUE 1

int compare_strings(char *char_string_ptr1,char *char_string_ptr2);

int main()
{
  char string1[20];
  char string2[20];
  printf("Compare Two Strings Without strcmp() Function \n");
  printf("Enter String1 : ");
  scanf("%s",string1);
  printf("Enter String2 : ");
  scanf("%s",string2);

  int ret_value = compare_strings(string1,string2);

  if(ret_value)
  {
	  printf("strings are same \n");
  }
  else
  {
	  printf("strings are not same \n");
  }
}

int compare_strings(char *char_string_ptr1,char *char_string_ptr2)
{
 printf("%s\n",char_string_ptr1);
 printf("%s\n",char_string_ptr2);
  int i = 0;
  int flag = 0;
  while((char_string_ptr1[i] == char_string_ptr2[i]))
  {

    if((char_string_ptr1[i] == '\0')&&(char_string_ptr2[i] == '\0'))
    {
        flag = 1;
        break;
    }
    i++;
  }
 
  if(flag == 0)
  {
    return FALSE;
   }
   else
  {
    return TRUE;
   }
}



/* 1. strings are stored in char string_array[] = "venkatesh"; (array index is left empty])
 * 2. the string_array[] is passed as address to char *strng_ptr;
 * 3. that char strng_ptr[] is referenced with index to read each charater in string;
 * 4. such two char string_pointers are compared with indexes;
 */


