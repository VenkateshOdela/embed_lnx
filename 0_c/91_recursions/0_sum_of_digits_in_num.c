

#include <stdio.h>

int sum =0;

int func(int number)
{
	if(number == 0)
	{
		return sum;
	}

	// if number is positive
	if(number > 0)
	{
		sum = sum + number%10;
		number = number/10;
	}
	else{ // if number is negative
		sum = sum - number%10;
		number = number/10;
	}


	return func(number);
}

int main()
{

	int number = -1234;
	int ret;
	ret = func(number);

	printf("%d \n",ret);

}
