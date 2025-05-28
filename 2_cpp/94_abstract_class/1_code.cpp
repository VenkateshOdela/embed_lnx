

/* abstract class */

#include <iostream>

using namespace std;


//abstract class
class base_shape
{
public:
	virtual void draw() = 0; // pure virtual function

};

class derived_square : public base_shape
{
public:
	void draw()
	{
		cout << "draw : square" << endl;
	}

};

class derived_rectangle : public base_shape
{
public:
	void draw()
	{
		cout << "draw : rectangle" << endl;
	}

};

int main()
{
	//base_shape obj; //error: cannot declare variable ‘obj’ to be of abstract type ‘base_shape’
	base_shape *ptr1 = new derived_square;
	base_shape *ptr2 = new derived_rectangle;

	ptr1->draw();
	ptr2->draw();

}

/*
draw : square
draw : rectangle
 *
 */

