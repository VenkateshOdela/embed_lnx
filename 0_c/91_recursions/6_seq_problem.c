

#include <stdio.h>

int arr[20];

int func(int a)
{
	static int i =3;

	if(a > 20)
	{
		printf("invalid input \n");
		return 0;
	}

    if( a == 0 || a == 1 || a == 2)
    {
    	switch(a)
    	{
    		case 0:
    		case 1:
    			return 1;
    			break;

    		case 2:
    			return 2;
    	}
    }

	if(i > a)
	{
		return arr[i-1];
	}

	arr[i] = arr[i-1]+arr[i-2]+arr[i-3]+i;
	i++;

	return func(a);
}

int main()
{
	int a=21;
	arr[0] = 1;
	arr[1] = 1;
	arr[2] = 2;

	int ret = func(a);

	printf("%d \n",ret);
}
