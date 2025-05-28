

/* constructor and destructor always in public
 *
 * if constructor and destructor are in private it will directly
 * compilation error
 *
 *
 *
 *
 * */




#include <iostream>

using namespace std;

class base
{
private:
	base()
	{
		cout << "constructor" << endl;
	}

	~base()
	{
		cout << "desstructor" << endl;
	}


};


int main()
{
	base obj;

}

/*
 * user@ubuntu:~/git_embd_linux/embd_linux/2_cpp/design_patters/singleton$ g++ 1_code.cpp
1_code.cpp: In function ‘int main()’:
1_code.cpp:24:7: error: ‘base::base()’ is private within this context
  base obj;
       ^~~
1_code.cpp:8:2: note: declared private here
  base()
  ^~~~
1_code.cpp:24:7: error: ‘base::~base()’ is private within this context
  base obj;
       ^~~
1_code.cpp:13:2: note: declared private here
  ~base()
  ^
 *
 *
 *
 */
