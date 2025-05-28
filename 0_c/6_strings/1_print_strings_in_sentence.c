
/*strtok : split the strings with space delimiter */


#include <stdio.h>
#include <string.h>

void search_strings(char *sentence, char *find_string)
{
	   char *arry_of_strings_ptr[50];

	   int string_count =0;
	   int string_repeated=0;

	   printf("%s \n \n",sentence);

	   arry_of_strings_ptr[string_count] = strtok(sentence," ");

	   while(arry_of_strings_ptr[string_count] != NULL)
	   {
		  printf("%s \n",arry_of_strings_ptr[string_count]);
		  string_count++;
		  arry_of_strings_ptr[string_count] = strtok(NULL," ");
	   }

	   printf("\n");
	   for(int i=0;i<string_count;i++)
	   {
		   if(strcmp(arry_of_strings_ptr[i],find_string) == 0)
		   {
			   string_repeated++;

		   }
	   }
	   printf("%s is repeated %d times \n",find_string,string_repeated);

}

int main()
{
   char sentence[] = "venkat viasat applied venkat rejected viasat applied viasat";

   char find_string[] = "venkat";

   search_strings(sentence,find_string);

}
