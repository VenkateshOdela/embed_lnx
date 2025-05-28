#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int find_word_count(char str[],char substring[])
{

	int m = strlen(str);
	int n = strlen(substring);
	int number_of_windows = m-n+1;
	int match_count=0;
	int word_count=0;
	//int arr[number_of_windows]=0;
	int *arr = malloc(number_of_windows*sizeof(int));


	for(int window_start =0; window_start<number_of_windows;window_start++)
	{
		match_count =0;
		for(int i=0;i<=n;i++)
		{
			if(substring[i] == str[window_start+i])
			{
				match_count++;
			}else{
				break;
			}
		}
		if(match_count == n)
		{
			arr[word_count] = window_start;
			word_count++;

		}
	}

	for(int i =0;i<number_of_windows;i++)
	{
		if(arr[i]!=0)
		{
			printf("found at index: %d \n", arr[i]);
		}

	}

	return word_count;
}

int main()
{
	char str[] = "sameer mahesh sameer got placed sameer hi sameer hi sameer ";
	char substring[]="sameer";

	int word_count = find_word_count(str,substring);
	printf("%s found %d times",substring,word_count);

}
