/*
 *  polymorphism :
 *
 *
 *  - with : virtual keyword;
 */

#include <iostream>
using namespace std;

class base {
   public:
    virtual void print() {
        cout << "Base print Function" << endl;
    }
};

class derived1: public base {
   public:
    void print() {
        cout << "Derived1 print Function" << endl;
    }
};

class derived2: public base {
   public:
    void print() {
        cout << "Derived2 print Function" << endl;
    }
};

class derived3: public base {
   public:
    void print() {
        cout << "Derived3 print Function" << endl;
    }
};

int main()
{

	base *ptr1 = new derived1();
	base *ptr2 = new derived2();
	base *ptr3 = new derived3();


	ptr1->print();
	ptr2->print();
	ptr3->print();

	delete ptr1;
	delete ptr2;
	delete ptr3;

}

/*
Derived1 print Function
Derived2 print Function
Derived3 print Function
 */
