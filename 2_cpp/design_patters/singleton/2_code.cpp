
// Implementation of Singleton Class
// in C++
//#include <bits/stdc++.h>

#include <iostream>
using namespace std;

class base{
  private:

  // member variables
  int id;
  string name;

  static base *instancePtr;

  // static pointer which will points
  //to the instance of this class

  // Default constructor in private
  base()
 {
    //cout << " default constructor in private" << endl;
  }

  public:

  // deleting copy constructor
 // base(const base& obj) = delete;

  /*
    getInstance() is a static method that returns an
    instance when it is invoked. It returns the same
    instance if it is invoked more than once as an instance
    of Singleton class is already created. It is static
    because we have to invoke this method without any object
    of Singleton class and static method can be invoked
    without object of class

    As constructor is private so we cannot create object of
    Singleton class without a static method as they can be
    called without objects. We have to create an instance of
    this Singleton class by using getInstance() method.
  */
  static base* getInstance()
  {
    // If there is no instance of class
    // then we can create an instance.
    if (instancePtr == NULL)
    {
      // We can access private members
      // within the class.
      instancePtr = new base();

      // returning the instance pointer
      return instancePtr;
    }
    else
    {
      // if instancePtr != NULL that means
      // the class already have an instance.
      // So, we are returning that instance
      // and not creating new one.
      return instancePtr;
    }
  }

  // sets values of member variables.
  void setValues(int id,
                 string name)
  {
    this->id = id;
    this->name = name;
  }

  // prints values of member variables
  void print()
  {
    cout << " id = " << id <<
            "name = "<< name << endl;
  }
};

// initializing instancePtr with NULL
base* base ::instancePtr = NULL;

// Driver code
int main()
{
  //base obj;
  base* ptr1 = base ::getInstance();

  // setting values of member variables.
  ptr1->setValues(1,"venkatesh");

  // printing values of member variables.
  ptr1->print();

  cout << "Address of ptr1 " << ptr1 << endl;

  cout << endl;

  base *ptr2 = base::getInstance();

  // setting values of member variables.
  ptr2->setValues(2,"suresh");

  // Printing values of member variables.
  ptr2->print();

  cout << "Address of ptr2 " << ptr2 << endl;
  return 0;
}
