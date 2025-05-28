


/* no swaping is done with this code*/

#include <stdio.h>

void swap_strings(char *s1, char *s2)
{
	 char *temp = s1;
	 s1 = s2;
	 s2 = temp;
}

int main()
{
	char str1[] = "venkatesh";
	char str2[] = "suresh";

	//char *str1 = "venkatesh";
	//char *str2 = "suresh";

	swap_strings(str1,str2);

	printf("str1 = %s \n",str1);
    printf("str2 = %s \n", str2);

}
