/*
 * 1.prints the address of the variable
 * 2.format specifier %p is used;
 */

#include <stdio.h>
 
int main()
{
    int x;
 
    // Prints address of x
    printf("%p\n", &x);
 
    return 0;
}

/*
 * 1.The output of this program can be different in different runs.
 *
 * 2.Note that the program prints address of a variable and a variable
 *   can be assigned different address in different runs.
*/

/* output :
 * 0x7ffe581f24d4
 *
 * observations :
 * 0x7f fe 58 1f 24 d4
 *
 * why address is only 6 bytes?
 *
 */
