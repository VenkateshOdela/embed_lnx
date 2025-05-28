

/* new - delete :
 * dynamic memory allocation for array of 20*4 bytes
 */

#include <iostream>

using namespace std;

int main()
{
	int *int_ptr = NULL;

	int_ptr = new int[20];

	int i;

	for(i = 0; i<20;i++)
	{
		int_ptr[i] = i;
		cout << "int_ptr[" << i<< "] = "<< int_ptr[i] << "\n";
	}

	delete [] int_ptr;

	for(i = 0; i<20;i++)
	{
		//char_ptr[i] = 'e';
		cout << "int_ptr[" << i<< "] = "<< int_ptr[i] << "\n";
	}


}
