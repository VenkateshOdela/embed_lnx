
/* unique pointer */

#include <iostream>
#include <memory>
#include <cstring>

using namespace std;

class base
{
public :
	int a;
};

void my_func()
{
    unique_ptr<base> ptr(new base);
    ptr->a = 10;
    cout << "ptr->a = " << ptr->a << endl;
    int x = 45;
    // ...
    // create a new unique_ptr ptr2 and move the ownership of ptr to ptr2
    //unique_ptr<int> ptr2 = move(ptr);
    // ptr is now empty
    if (x == 45)
        return; // no memory leak anymore!
    // ...
    // you don't have to delete the pointer variable ptr.
    // the memory will automatically be freed after the return statement.
}

int main()
{
    my_func();
    return 0;
}
