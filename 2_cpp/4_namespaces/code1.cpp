/* namespaces :
 * For example, you might be writing some code that has a function called xyz() and
 * there is another library available which is also having same function xyz().
 * Now the compiler has no way of knowing which version of xyz()
 * function you are referring to within your code.
 *
 * advantages of namespaces :
 * Avoiding naming conflicts:
 */

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

int main () {
   // Calls function from first name space.
	// prepend (::) the namespace
	//scope resolution operator (::)
   first_space::func();

   // Calls function from second name space.
   // prepend (::) the namespace
   second_space::func();

   return 0;
}

/*
 *
 */
