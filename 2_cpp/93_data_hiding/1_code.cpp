
/* data hiding
 *
 * public data member is aceesable through dot operator
 *
 *
 * */

#include <iostream>

using namespace std;

class base
{
public:
	int a;
};

int main()
{
	base obj;
	// access the data member out of class
	obj.a = 5;
}
