/*
 *   a constructor is called when object is created;
 *
 */
#include <iostream>

using namespace std;

class MyClass {
public:
	int a =80;

    MyClass() {
        cout << "Object created." << endl;
        a = 20;
    }

    ~MyClass() {
        cout << "Object destroyed." << endl;
    }
};

int main()
{
    MyClass myObj; // Create an object of class MyClass

    cout << myObj.a << endl;
    return 0;
}
