
/* new - delete :
 * dynamic memory allocation for array of 20*1 bytes
 */

#include <iostream>

using namespace std;

int main()
{
	char *char_ptr = NULL;

	char_ptr = new char[20];

	int i;

	for(i = 0; i<20;i++)
	{
		char_ptr[i] = 'e';
		cout << "char_ptr[" << i<< "] = "<< char_ptr[i] << "\n";
	}

	delete char_ptr;

	for(i = 0; i<20;i++)
	{
		//char_ptr[i] = 'e';
		cout << "char_ptr[" << i<< "] = "<< char_ptr[i] << "\n";
	}
}
