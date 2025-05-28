/*
 * friend function allows to access the data members from the outside of class.
 *
 *
 */

#include <iostream>

using namespace std;

class base
{

public :

	base(int a)
	{
		this->a = a;
	}
	friend void print(base &obj);

   private:
	int a;


};

//friend void print(base &obj);

void print(base &obj)
{
	cout << obj.a << endl;
}


int main()
{
	base obj1(65);
	base obj2(20);

	//cout << obj1.a << endl; // invalid access the private data member with dot operator
	print(obj1);
	print(obj2);


}
