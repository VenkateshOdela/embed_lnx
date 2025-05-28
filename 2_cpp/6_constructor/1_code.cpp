/*
 *  when object is created, constructor is called;
 *  when object is destroyed/outofscope ,destructor is called;
 *
 *  constructor have same name as class name;
 *  constructors are used to initiliaze the data members;
 *
 */

#include <iostream>

using namespace std;

class my_baseclass
{

public :
	int a;
	my_baseclass()
	{
		this->a=4; // constructor is used to initiliaze the object
		cout <<"contructor is called" << endl;

	}

	~my_baseclass()
	{
		cout <<"destructor is called" << endl;
	}
};

int main()
{

	my_baseclass obj;// when obj is created constructor is called;

	cout << "a = " << obj.a << endl;

	// here is object goes out of scope so destructor is called
	return 0;
}
/*
contructor is called
a = 60
destructor is called
*/
