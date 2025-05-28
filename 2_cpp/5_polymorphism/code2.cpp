/* Runtime Polymorphism
 *	- virtual functions;
 *
 *	Runtime polymorphism is achieved using a combination of function overriding and virtual functions.
 *
 *	Runtime polymorphism occurs when functions are resolved at runtime rather than compile time.(late binding)
 *
 *	So, to achieve runtime polymorphism in C++, you need to declare the function as virtual in the base class, and override it in the derived class.
 *
 */

/*
 * A virtual function is a member function in the base class that we expect to redefine in derived classes.
 */
#include <iostream>
using namespace std;

class Base {
   public:
    virtual void print() {
        cout << "Base print Function" << endl;
    }
};

class Derived : public Base {
   public:
    void print() {
        cout << "Derived print Function" << endl;
    }
};

int main() {
    Derived derived_obj;

    // pointer of Base type that points to derived1
    Base* ptr = &derived_obj;

    // calls member function of Derived class
    ptr->print(); //  this print function defination is decided at runtime;

    return 0;
}

/*
 * virtual function;
 *
 * op without virtual :
 *  base print function
 *
 * op with virtual:
 *  dervied print function;
 */

