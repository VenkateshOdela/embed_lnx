#if 0

/* weak pointers */

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
    // Initialise the pointer P1 as you would normally do with a normal pointer
    // This'll print the title of the article
    strcpy(ptr1->name,"venkatesh");
    cout << ptr1->name << endl;

    weak_ptr<base> ptr2;
    ptr2 = ptr1;

    // This'll print also print the same title as printed by P1
    cout << ptr2->name << endl;
    // You can print the number of pointers pointing to that location with the help of the use_count() method
    cout << ptr1.use_count() << endl;
    // This'll print 1 as the weak pointer is not counted as an owner.
    // The memory gets released once the pointer P1 releases it or goes out of scope.

    return 0;
}
#endif

#include <memory>
#include <iostream>
using namespace std;

class Articles
{
public:
    char title[30];


    // Implementation of the Articles class.

}

int main()
{

    shared_ptr<Articles> P1(new Articles());
    // Initialise the pointer P1 as you would normally do with a normal pointer
    // This'll print the title of the article
    cout << P1->title << endl;

    weak_ptr<Articles> P2;
    P2 = P1;

    // This'll print also print the same title as printed by P1
    cout << P2->title << endl;
    // You can print the number of pointers pointing to that location with the help of the use_count() method
    cout << P1.use_count() << endl;
    // This'll print 1 as the weak pointer is not counted as an owner.
    // The memory gets released once the pointer P1 releases it or goes out of scope.

    return 0;
}
