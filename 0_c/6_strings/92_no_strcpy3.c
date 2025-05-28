/*
 * The  strcpy()  function  copies the string pointed to by src, including the terminating null
   byte ('\0'), to the buffer pointed to by dest.

 *   The strncpy() function is similar, except that at most n bytes of src are copied.   Warning:
       If  there is no null byte among the first n bytes of src, the string placed in dest will not
       be null-terminated.

       If the length of src is less than n, strncpy() writes  additional  null  bytes  to  dest  to
       ensure that a total of n bytes are written
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 char *my_strcpy(char *dest, const char *src);

 char *my_strncpy(char *dest, const char *src, size_t n);

 char *my_strcpy(char *dest, const char *src)
 {
	 int i;
	 int length =0;
	 const char *src_ptr = src;

	 while(*src_ptr != '\0')
	 {
		 src_ptr++;
		 length = length +1;
	 }

	 char *dest_allocated = (char *)malloc(length*sizeof(char));

	 src_ptr = src;
	 char *temp_ptr = dest_allocated;

	 while(*src_ptr != '\0')
	 {
		 *temp_ptr = *src_ptr;
		 src_ptr++;
		 temp_ptr++;
	 }

	 *temp_ptr = '\0';

	 return dest_allocated;
 }

int main()
{
	char src_str[] ="venkatesh";

	char des_str[50]={0};

	char *des_ptr = NULL;

	des_ptr =my_strcpy(des_str,src_str);

	printf("%s \n",des_str);

	printf("%s \n",des_ptr);

}
