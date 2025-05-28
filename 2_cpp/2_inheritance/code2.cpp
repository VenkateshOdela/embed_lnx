// C++ program to demonstrate function overriding

#include <iostream>
using namespace std;

class base {
   public:
    void print() {
        cout << "Base Function" << endl;
    }
};

//class derived : public base {
class derived {
   public:
    void print() {
        cout << "Derived Function" << endl;
    }
};

int main() {

	/*calling print in base */
	base base_obj;
	base_obj.print();

	/*calling print in derived */
    derived derived_obj;
    derived_obj.print();

    /* calling print in base class from derived class object */
    //derived_obj.base::print();
    return 0;
}
