#include <iostream>

using namespace std;


class base
{
	public:

	void print(int n)
	{
		cout << "n =" << n << endl;
	}

	void print(const char *s)
	{
		cout << "name = " << s << endl;
	}

};

int main()
{
	base obj;

	obj.print(10);

	obj.print("venkatesh");

	return 0;
}
