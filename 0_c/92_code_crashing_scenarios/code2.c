/*
 *  crash type : stack smash;
 *
 *  accessing the array index more than it's size,will crash the code;
 *
 *
 */

#include <stdio.h>

int main()
{
	int arr[3] = {1,2,3};

	arr[3] = 4;  //stack smash

}
