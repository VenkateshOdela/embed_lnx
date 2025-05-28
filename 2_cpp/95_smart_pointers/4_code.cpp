
/*
 *  comparison of raw pointers and smart pointers
 */

#include <memory>
#include <iostream>
#include <cstring>

using namespace std;

class base
{
public:
    char name[30];

    base(const char *s)
    {
    	strcpy(this->name,s);
    }
};

void UsingRawPointer()
{
    // Using a raw pointer -- not recommended.
    base *ptr = new base("raw pointer : Nothing on You ");

    // Use articles...
    cout << ptr->name << endl;

    // Don't forget to delete it!
    delete ptr;
}


void UsingSmartPointer()
{
    // Declare a smart pointer and pass it to the raw pointer via the constructor.
    unique_ptr<base> ptr2(new base("smart pointer Nothing on You"));

    // Use articles2...
   cout << ptr2->name << endl;
    //...

} // ptr2 is deleted automatically here.

int main()
{
	UsingRawPointer();

	UsingSmartPointer();

}
