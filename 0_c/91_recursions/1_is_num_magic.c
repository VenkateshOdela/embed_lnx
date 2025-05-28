
#include <stdio.h>

int sum =0;

int func(int number)
{
	if( (number == 0) && (sum == 1))
	{
		//printf("+1");
		return 1;
	}

	if( (number == 0) && (sum >= 10))
	{
		number = sum; // new number for next iteration
		sum = 0; // making sum = 0 for next iteration;
		return func(number);
	}

	if( (number == 0) && (sum < 10 ))
	{
		return 0 ;
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

	int number = 1291;
	int ret;
	ret = func(number);

	printf("%d \n",ret);

}
