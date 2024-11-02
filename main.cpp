#include <iostream>
#include <string>
#include "LongInteger.cpp"
using namespace std;


int main() {
    string inout;

    LongInteger num;
    long long int it;
    cin >> num >> it;

    // cout << LongInteger::jacobi( num1, stoll(num.value)) << endl;
    // cout << LongInteger::soloveyShtrassen(num, it);

    // bool res = LongInteger::soloveyShtrassen(stoll(num.value), it);
    // LongInteger::output_Solovay_Strassen(res);

    cout << LongInteger::inverseValue(num, it) << endl;

    return 0;
}
