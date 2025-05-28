#include <stdio.h>

void divisable_by_2();

void divisable_by_4();

void divisable_by_8();
int main()
{

	int num;
	printf("enter a number :");
	scanf("%d",&num);

	divisable_by_2(num);

	divisable_by_4(num);

	divisable_by_8(num);

}
/*
 * divisable_by_2 logic :
 * 	- the first right most bit is must be zero
 *
 */
void divisable_by_2(int num)
{
	if((num & 1)== 0)
	{
		printf("%d is divisable by 2 \n",num);
	}
	else{
		printf("%d is NOT divisable by 2\n",num);

	}

	printf("\n");

}

/*
 * divisable_by_4 logic :
 * 	- the first two right most bits is must be zero
 *
 */

void divisable_by_4(int num)
{
	if((num & 3) == 0)
	{
		printf("%d is divisable by 4\n",num);
	}
	else{
		printf("%d is NOT divisable by 4\n",num);

	}
	printf("\n");

}

/*
 * divisable_by_8 logic :
 * 	- the first three right most bits is must be zero
 *
 */

void divisable_by_8(int num)
{
	if((num & 7) == 0)
	{
		printf("%d is divisable by 8\n",num);
	}
	else{
		printf("%d is NOT divisable by 8\n",num);

	}
	printf("\n");

}
