

/* swap strings */

#include <stdio.h>

void swap_strings(char **str1_dpntr, char **str2_dpntr)
{
	 char *temp = *str1_dpntr;
	 *str1_dpntr = *str2_dpntr;
	 *str2_dpntr  = temp;
}

int main()
{
	char str1[] = "venkatesh";
	char str2[] = "suresh";

	char *str1_pntr = str1;
	char *str2_pntr = str2;

	swap_strings(str1_pntr,str2_pntr);

	printf("str1 = %s \n",str1);
    printf("str2 = %s \n", str2);

}
