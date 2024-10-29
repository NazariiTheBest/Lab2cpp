#include <iostream>
#include <string>
#include "LongInteger.cpp"
using namespace std;


int main() {
    LongInteger number1, number2;
    cin >> number1 >> number2;
    cout << LongInteger::katsarubaMethod(number1, number2) << "\n\n";
    cout << LongInteger::toom_cook(number1.value, number2.value) << "\n\n\n";
    cout << LongInteger::katsarubaMethod(number1, number2) << "\n\n";
     return 0;
}