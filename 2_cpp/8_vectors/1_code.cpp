
/*
 * vectors : array size grow size can be increased in run time
 */

#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector <int> array {10,20,30};

	cout << " array.size = " << array.size() << endl;

	cout << array.at(0) << endl;
	cout << array.at(1) << endl;
	cout << array.at(2) << endl;

	array.push_back(40);

	cout << " array.size = " << array.size() << endl;

	cout << array.at(3) << endl;

}
