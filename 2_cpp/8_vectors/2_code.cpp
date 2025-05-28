
/* vectors provide boundary check */


#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector <int> array {10,20,30};

	cout << array.at(0)<< endl;
	cout << array.at(1)<< endl;
	cout << array.at(2)<< endl;

	cout << array.at(3)<< endl; // bounday check throws an exception

	return 0;
}
/*
user@ubuntu:~/git_embd_linux/embd_linux/2_cpp/8_vectors$ ./a.out
10
20
30
terminate called after throwing an instance of 'std::out_of_range'
  what():  vector::_M_range_check: __n (which is 3) >= this->size() (which is 3)
Aborted (core dumped)
*/

