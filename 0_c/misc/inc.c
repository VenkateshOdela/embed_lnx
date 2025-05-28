#include <stdio.h>

int main()
{
	int a = 20;

	int b = 25 % 10; // give remainder 5
	/*
	 * 10)25(2
	 *    20
	 *    __
	 *    5
	 *
	 */

	int c = 25 /10; // gives quotient 2

	printf("b:%d\n",b);
	printf("c:%d\n",c);

	int d=25 ;
	d = (++d)%10; // 26%10 = 6

	printf("d:%d\n",d);

}

/*
 * b:5 c:2
 *
 *
 */
