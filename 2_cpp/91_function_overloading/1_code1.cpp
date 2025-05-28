
/* function overloading */


#include <iostream>

using namespace std;

void print(int n)
{
	cout << "number =" << n << endl;
}

void print(const char *s)
{
	cout << "name = "<< s << endl;
}

int main()
{
	print(10);
	print("venkatesh");
	return 0;
}
