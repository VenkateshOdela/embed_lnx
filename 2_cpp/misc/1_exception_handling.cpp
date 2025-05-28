#include <iostream>

using namespace std;

int main() {
    int dividend, divisor, quotient;

    cout << "Enter the dividend: ";
    cin >> dividend;

    cout << "Enter the divisor: ";
    cin >> divisor;

    try {
        if (divisor == 0) {
            throw "Divide by zero error!";
        }
        quotient = dividend / divisor;
        cout << "Quotient = " << quotient << endl;
    }
    catch (const char* error) {
        cout << "Error: " << error << endl;
    }

    return 0;
}
