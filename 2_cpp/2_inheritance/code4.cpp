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
    ptr->print();

    return 0;
}

/*
 * virtual function;
 */
