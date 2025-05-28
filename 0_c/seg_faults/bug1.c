#include<stdio.h>


int main()
{

	char *str = "12345";
	printf("%s", *str);
 
	return 0;
}

/*
int main()
{

	char *str = "12345";
	printf("%s", str);
 
	return 0;
}*/

/*
*str point to the first charecter of "12345".
So str[0]='1', when %d is used to display then it is converted to intiger or  ascii value 49.

But with accessing %s, the str is pointer of char*, the location of string in memory, cannot be converted to string type.. Since it point to single value but accessing more than that causes segmentation error...
Edit:
%s takes that *str value as address for retrieving string.. So cause segment error..
For some more clarity, see ~ swim ~answer below..
*/
