
/* when new is called to create memory for class , it's constructor is also called */
#include <iostream>
//#include <cstring>
#include <string>

using namespace std;

class base
{
public:
  int a;
  string name;

  myclass(int a,string name){
	  this->a = a;
	  this->name = name;
      cout << "Constructor called" << endl;
  }

  ~myclass() {
      cout << "Destructor called" << endl;
  }
};

int main()

{
  base* ptr = new myclass(42,"venkatesh"); // new allocate memory for class and call constructor;

  cout << "a = " << ptr->a <<  endl;
  cout << "str : " << ptr->str << endl;

  delete ptr; // call destructor and deallocate memory

  return 0;
}
