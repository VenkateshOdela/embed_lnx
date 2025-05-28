/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <string.h>

struct abc{
    
    int a;
    char s[6];
    void  *pvValue;               
    
};
int main()
{
    struct abc obj;
    struct abc *sptr= &obj;
    
    *sptr->s = '\0';
    
    for (int i=0; i<6;i++)
    {
        printf(".......%c \n",(sptr->s[i]) );
    }
    
    strncat(sptr->s,"carriettt",9);
    
    printf("%s \n",sptr->s);
    
    printf("sizeof : %ld \n",sizeof(sptr->s));
    printf("strlen : %ld \n",strlen(sptr->s));
    
}
/*
int main()
{
    //printf("Hello World");
    struct abc obj;
    struct abc *sptr = &obj;
    sptr->a = 10;
    //printf("%d \n",sptr->a);
    
    *sptr->s = 0;
    printf("Destination:%ld \n",strlen(sptr->s));
    
    //*sptr->s = "venkat";
    char *str = "venkateshhhhhhhhh";
    strcpy(sptr->s,str);
    printf("Destination:%ld \n",strlen(sptr->s));

    
   
    
    sptr->pvValue = (void *)str;
    //strcpy((char *)sptr->pvValue,str);
    printf("sourece :%ld \n",strlen((char *)sptr->pvValue));
     
    

    
    return 0;
}
*/

