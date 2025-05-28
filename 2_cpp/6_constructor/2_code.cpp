/*
 *  copy constructor
 */

// C++ program to demonstrate the working
// of a COPY CONSTRUCTOR
#include <iostream>
using namespace std;

class baseclass {
    public:
    int a, b,c,d;

    //default constructor
    baseclass(int x1, int y1)
    {
        a = x1;
        b = y1;
    }

    // Copy constructor
    baseclass(baseclass &p1)
    {
        c = p1.a;
        d = p1.b;
    }

};

int main()
{
    baseclass obj1(10, 15); // Normal constructor is called here
    baseclass obj2 = obj1; // Copy constructor is called here

    // Let us access values assigned by constructors
    cout << "p1.a= " << obj1.a
         << ", p1.b = " << obj1.b;
    cout << "\np2.c = " << obj2.c
         << ", p2.d = " << obj2.d;
    return 0;
}
