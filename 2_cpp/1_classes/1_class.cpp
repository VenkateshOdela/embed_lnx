#include <iostream>

using namespace std;

class myclass {
public:
    int publicVar; // public variable

    void setPrivateVar(int a)
    { // public method to set private variable
        privateVar = a;
    }

    void setProtectedVar(int a) { // public method to set protected variable
        protectedVar = a;
    }

    int getPrivateVar()
    {
    	return privateVar;
    }

    int getProtectedVar() {
    	return privateVar;
    }



private:
    int privateVar; // private variable

protected:
    int protectedVar; // protected variable
};

int main() {
    myclass myobj;

    myobj.publicVar = 1; // only public data member can be accessed through access operator (.)

    // myobj.privateVar = 2;  // INVALID
    // myobj.protectedVar = 3; // INVALID

    myobj.setPrivateVar(2); // accessing private variable through same class public member function
    myobj.setProtectedVar(3); // accessing protected variable through same class public member function

    cout << "Public variable: " << myobj.publicVar << endl;
    cout << "Private variable: " << myobj.getPrivateVar() << endl; // accessing private variable through public method
    cout << "Protected variable: " << myobj.getProtectedVar() << endl; // accessing protected variable through public method

    return 0;
}
