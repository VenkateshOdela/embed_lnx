
/* this pointer :
 *
 *  - this pointer has only class scope;
 *  - all class members access is done via this pointer;
 */
#include <iostream>

using namespace std;

class base
{
public :
	int id;
	string name;

	base(int id, string name)
	{
		this->id = id;
		this->name = name;
		cout << "constructor is called" << endl;
	}

	void print()
	{
		cout << this->id << endl;
		cout << this->name << endl;
	}

	~base()
	{
		cout << "destructor is called" << endl;
	}
};


int main()
{
	base obj1(10,"venkatesh");
	base obj2(20,"naresh");

	obj1.print();
	obj2.print();
}
