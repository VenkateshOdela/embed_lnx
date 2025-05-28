
/*
 * shared ptr
 */

#include <memory>
#include <iostream>
#include <cstring>

using namespace std;

class base
{
    public:
    char name[30];
};

int main()
{

    shared_ptr<base> ptr1(new base());
    // Initialise the pointer ptr1 as you would normally do with a normal pointer
    // This'll print the title of the article

    strcpy(ptr1->name,"venkatesh");
    cout << ptr1->name << endl;


    shared_ptr<base> ptr2;
    ptr2 = ptr1;

    // This'll print also print the same title as printed by ptr1
     cout << ptr2->name << endl;
    // You can print the number of pointers pointing to that location with the help of the use_count() method
     cout << ptr1.use_count() << endl;
    // This'll print 2 as Reference Counter is 2
    // The memory will only be freed when both the pointers P1 and P2 have gone out of scope or released the ownership of the location.

    return 0;
}
