
/* total program is halted due divide by zero */

#include <iostream>
using namespace std;

int main()
{

    int a= 10 ,b =0;

    // Dividing the number a by zero, so the program will compile easily
    // but run time error will be generated.
    cout << a/b;

    // this line is not executed as core dumped in previous line
    cout << "venkatesh odela" << endl;
    return 0;
}
