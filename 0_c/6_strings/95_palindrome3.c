

#include <stdio.h>
#include <string.h>

int checkpalindrome(char *s)
{
	int left = 0;
	int right = strlen(s) -1;

	return helper(s,left,right);

}

int helper(char *s, int left, int right)
{
	if(left>right)
	{
		return 1;
	}

	if(s[left] == s[right])
	{
		return helper(s,++left,--right);
	}
	else{
		return 0;
	}
}


int main()
{

	char str[] = "madam";
	int ret = checkpalindrome(str);

	if(ret ==1)
	{
		printf("yes \n");
	}
	else{
		printf("no \n");
	}
}
