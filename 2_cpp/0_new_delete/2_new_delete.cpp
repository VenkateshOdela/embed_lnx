

/* new - delete :
 * dynamic memory allocation of one byte */

#include <iostream>

using namespace std;

int main()
{
	char *ptr = NULL;

	ptr = new char; // requesting memory

	*ptr = 10;

	cout << " ptr :" << *ptr << "\n" ;

	delete ptr; // freeing memory
}
