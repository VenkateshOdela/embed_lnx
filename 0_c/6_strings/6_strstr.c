/*
 * NAME
       strstr, strcasestr - locate a substring

SYNOPSIS
       #include <string.h>

       char *strstr(const char *haystack, const char *needle);

DESCRIPTION
       The  strstr()  function  finds  the  first  occurrence of the substring needle in the string
       haystack.  The terminating null bytes ('\0') are not compared.


RETURN VALUE
       These functions return a pointer to the beginning of the located substring,
       or NULL  if  the substring is not found.
 *
 *
 *
 */

/*
 *
 *  finding the number of occurences of word in given string;
 */

#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "suresh venkat applied rejected rejected applied venkat suresh suresh";
    char find_word[]="venkat";
    int i,count=0;

    for (i = 0; str[i] != '\0'; i++)
    {
        if (strstr(&str[i], find_word) == &str[i]) {
            count++;
        }
    }

    printf("The word '%s' occurs %d times in the given string.\n", find_word, count);

    return 0;
}
