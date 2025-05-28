
/* we can access the private members with in class */


#include <iostream>

using namespace std;

class base
{

public:
	base *ptr = new base();
private:
	base()
	{
		cout << "constructor" << endl;
	}

	~base()
	{
		cout << "desstructor" << endl;
	}


};


int main()
{
	//base obj;

}
