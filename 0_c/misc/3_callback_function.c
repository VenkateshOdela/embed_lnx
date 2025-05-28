#include <stdio.h>

void myfunction()
{
	printf("this is my function \n");
}

void callbackfunction(void (*ptr)())
{
	(*ptr)();

}
int main()
{
	void *ptr = &myfunction;
	callbackfunction(ptr);
	return 0;

}
