

/*
 * write  function pointer to return sum of two numbers
 */

#include <iostream>

using namespace std;

int sum(int a, int b)
{
	return a+b;
}

int main()
{
	int a = 10;
	int b = 20;
	int ret;

	int (*func_pntr)(int, int);

	func_pntr = &sum;

	ret = (*func_pntr)(a,b);

	cout << ret << endl;
}




