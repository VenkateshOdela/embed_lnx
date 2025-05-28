
/* The using directive
You can also avoid prepending of namespaces with the using namespace directive.
This directive tells the compiler that the subsequent code is making use of names in the specified namespace */

#include <iostream>
using namespace std;

// first name space
namespace first_space {
   void func() {
      cout << "Inside first_space" << endl;
   }
}

// second name space
namespace second_space {
   void func() {
      cout << "Inside second_space" << endl;
   }
}

using namespace first_space;
int main () {
   // This calls function from first name space.
   func();

   return 0;
}

/*
 * Inside first_space
 */
