
/* program is not halted due  to divide by zero
 * - it throws divide by zero exception and do remaining work;
 *
 * */



#include <iostream>
using namespace std;

int main()
{

    int a = 10, b =0;

    // Dividing the number a by zero, so the program will compile easily
    // but run time error will be generated.
    try
    {
    	if(b == 0)
    	{
    		throw "divide by zero exception";
    		cout << a/b << endl;
    	}

    }

    catch(const char*s)
    {
    	cout << s << endl;
    }

    // this line is executed even though there is divide by zero issue
    cout << "venkatesh odela" << endl;
    return 0;
}
