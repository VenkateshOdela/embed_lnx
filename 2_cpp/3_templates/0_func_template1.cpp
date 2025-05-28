#include <iostream>
using namespace std;

// Function template to find the maximum value between two values of any data type
template<typename T>
T getMax(T a, T b) {
    return (a > b) ? a : b;
}

int main() {
    // Testing the function with integer values
    int intResult = getMax<int>(3, 5);
    cout << "Max integer value is: " << intResult << endl;

    // Testing the function with float values
    float floatResult = getMax<float>(3.14f, 1.618f);
    cout << "Max float value is: " << floatResult << endl;

    // Testing the function with string values
    string stringResult = getMax<string>("hello", "world");
    cout << "Max string value is: " << stringResult << endl;

    return 0;
}
