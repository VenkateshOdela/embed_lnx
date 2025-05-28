// C++ program to demonstrate inheritance

#include <iostream>
using namespace std;

// base class
class base {

   public:
    void func1() {
        cout << "this is func1" << endl;
    }

    void func2() {
        cout << "this is func2" << endl;
    }
};

// derived class
class derived : public base {

   public:
    void func3() {
        cout << "this is func3" << endl;
    }
};

int main() {
    // Create object of the Dog class
    derived derived_obj;

    // Calling members of the base class
    derived_obj.func1();
    derived_obj.func2();

    // Calling member of the derived class
    derived_obj.func3();

    return 0;
}

/*
 * I can eat!
I can sleep!
I can bark!
 *
 */
