
/*
 * compile time polymorphism
 *   - function overloading example;(early binding);
 * */

#include <iostream>
using namespace std;
class base
{

    public:
        void print(int a)
        {
           cout << "number : " << a << endl;
        }
        void print(const char *s)
        {
        	cout << "string : " << s << endl;
        }


};
int main() {
    base obj;
    obj.print(10);
    obj.print("venkatesh");
    return 0;
}

/*
 * number : 10
 * string : venkatesh
 *
 *
 * which function(print) to be executed is decided at compile time only;
 */

