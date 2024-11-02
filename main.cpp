#include <iostream>
#include <string>
#include "LongInteger.cpp"
using namespace std;

int main() {
    cout << "Enter number of operation you want to execute: " << endl;
    cout << "Multiplication of non-negative integers by the Karatsuba method - 1" << endl;
    cout << "Multiplication of non-negative integers by the Toomey-Cook method - 2" << endl;
    cout << "Multiplication of non-negative integers by the Schonhage-Strassen method - 3" << endl;
    cout << "Calculating the inverse with high accuracy (Cook's algorithm) (NOT IMPLEMENTED, WIP) - 4" << endl;
    cout << "Division of integers by Cook's algorithm - 5 (NOT IMPLEMENTED, WIP)" << endl;
    cout << "Checking the prime number by Lemer's method - 6" << endl;
    cout << "Checking the prime number using the Rabin-Miller method - 7" << endl;
    cout << "Checking the prime number using the Solovey-Strauss method - 8" << endl;
    cout << "Checking the prime number by the Frobenius method - 9" << endl;
    cout << "Stop executing - 0" << endl;

    int oper;
    cin >> oper;
    LongInteger number1, number2, number;
    long long int it;
    bool res;
    switch (oper) {
        case 1:
            cin >> number1 >> number2;
            cout << "OUTPUT:    ---->       " << LongInteger::katsarubaMethod(number1, number2) << endl;
        break;
        case 2:
            cin >> number1 >> number2;
            cout << "OUTPUT:    ---->       " << LongInteger::toom_cook(number1, number2) << endl;
        break;
        case 3:
            cin >> number1 >> number2;
            cout << "OUTPUT:    ---->       " << LongInteger::ShonhageStrassen(number1, number2) << endl;
        break;
        case 6:
            cin >> number >> it;
            res = LongInteger::LamerTest(number, it);
            LongInteger::output_prime(res);
        break;
        case 7:
            cin >> number >> it;
            res = LongInteger::Miller_RabinTest(number, it);
            LongInteger::output_prime(res);
        break;
        case 8:
            cin >> number >> it;
            res = LongInteger::soloveyShtrassen(number, it);
            LongInteger::output_prime(res);
        break;
        case 9:
            cin >> number >> it;
            res = LongInteger::frobeniusTest(number, it);
            LongInteger::output_prime(res);
        break;
    }

    return 0;
}
