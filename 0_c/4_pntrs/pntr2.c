/*
 * 1.C program to demonstrate declaration of pointer variables.
 *
 */

#include <stdio.h>
int main()
{
    int x = 10;
 
    // 1) Since there is * in declaration, ptr
    // becomes a pointer variable (a variable
    // that stores address of another variable)

    // 2) Since there is int before *, ptr is
    // pointer to an integer type variable
    int *ptr;
 
    // & operator before x is used to get address
    // of x. The address of x is assigned to ptr.
    ptr = &x;

    printf("ptr = %x\n",ptr);

    printf("ptr = %p\n",ptr);
 
    return 0;
}

/* output :
 * ptr = bf669a0c
 * ptr = 0x7ffcbf669a0c
 *
 * ptr = bf 66 9a 0c : %x is used = 4 bytes
 * ptr = 0x7f fc bf 66 9a 0c : %p is used = 6 bytes;
 *
 */
