#include <iostream>
#include <string>
#include "LongInteger.cpp"
using namespace std;


int main() {
    string input;

    // Read two LongInteger values for multiplication
    cout << "Enter two numbers for multiplication: ";
    cin >> input; // Read first number
    LongInteger number1(input);

    cin >> input; // Read second number
    LongInteger number2(input);

    // Output the results of various multiplication methods
    cout << "Katsaruba multiplication: " << LongInteger::katsarubaMethod(number1, number2) << "\n";
    cout << "Toom-Cook multiplication: " << LongInteger::toom_cook(number1.value, number2.value) << "\n";
    cout << "Shonhage-Strassen multiplication: " << LongInteger::ShonhageStrassen(number1.value, number2.value) << "\n";

    // Read a LongInteger number and iterations for primality tests
    cout << "Enter a number for primality test: ";
    cin >> input; // Read the number for primality test
    LongInteger number3(input);

    long long iter;
    cout << "Enter number of iterations for the Solovay-Strassen test: ";
    cin >> iter; // Read iterations

    // Perform and output the result of the Solovay-Strassen test
    cout << "Is " << number3 << " prime? " << (LongInteger::SolovayStrassen(number3, iter) ? "true" : "false") << "\n";

    return 0;
}
