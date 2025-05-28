/* data hiding
 *
 * private data member is not accessable through .dot operator
 *
 *
 *  */

#include <iostream>

using namespace std;

class base
{
private:
	int a;
};

int main()
{
	base obj;
	obj.a = 5; // invalid access of the private data member;
}


/*
user@ubuntu:~/git_embd_linux/embd_linux/2_cpp/93_data_hiding$ g++ 2_code.cpp
2_code.cpp: In function ‘int main()’:
2_code.cpp:16:6: error: ‘int base::a’ is private within this context
  obj.a = 5;
      ^
2_code.cpp:10:6: note: declared private here
  int a;
      ^
 *
 */
