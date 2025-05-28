
/*
 *  1. understanding what default values are assigned
 *     for unintialized global and local variables;
 *
 */

#include <stdio.h>

int global_var;

void func();

void func()
{
    int func_var;
    
    printf("func_var =  %d\n",func_var);
}

int main()
{
 int local_var;

 printf("global_var = %d\n",global_var);

 printf("local_var =  %d\n",local_var);
 
 func();
 
}

/*
 * 1.if global and static variables are not initialized ,the default
 *   values assigned to them is based on compiler we use;
 *
 */



