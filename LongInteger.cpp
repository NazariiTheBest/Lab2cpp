#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <memory>
using namespace std;

class LongInteger {
public:
    string value;
    long int base = 10;
    vector<int> coefficients;

    LongInteger() {
        value = "0";
    }
    LongInteger(string x) {
        value = x;
    }
    LongInteger(int x) {
        value = to_string(x);
    }

    LongInteger operator+(const LongInteger& x) {
        // Вирівнюємо рядки, щоб мали однакову довжину
        long max_len = max(value.length(), x.value.length());
        string num1 = value, num2 = x.value;
        fillValues(num1, num2);

        int carry = 0;
        string result;

        for (int i = max_len - 1; i >= 0; i--) {
            int sum = (num1[i] - '0') + (num2[i] - '0') + carry;
            result = to_string(sum % base) + result;
            carry = sum / base;
        }
        if (carry) {
            result = to_string(carry) + result;
        }

        // Видаляємо провідні нулі, якщо є
        result.erase(0, std::min(result.find_first_not_of('0'), result.size() - 1));

        return LongInteger(result);
    }
    LongInteger operator-(const LongInteger& x) {
        string num1 = value, num2 = x.value;
        int diff;
        long max_len = max(num1.length(), num2.length());
        string res;
        fillValues(num1, num2);

        string xStr, yStr;
        if (num1 > num2) {
            xStr = num1;
            yStr = num2;
        } else {
            xStr = num2;
            yStr = num1;
        }
        for (int i = max_len - 1; i >= 0; i--) {
            diff = (xStr[i] - '0') - (yStr[i] - '0');

            if (diff >= 0) {
                res.insert(0, to_string(diff));
            } else {
                int prev = i - 1;
                while (prev >= 0) {
                    xStr[prev] = (base + (xStr[prev] - '0') - 1) % base + '0';

                    if (xStr[prev] != '9') {
                        break;
                    } else {
                        prev-= 1;
                    }
                }
                res.insert(0, std::to_string(diff + base));
            }
        }
        return LongInteger(res.erase(0, std::min(res.find_first_not_of('0'), res.size() - 1)));
    }
    LongInteger operator==(const LongInteger& x) {
        return value == x.value;
    }
    LongInteger operator!=(const LongInteger& x) {
        return !(value == x.value);
    }
    LongInteger& operator=(string x) {
        *this = LongInteger(move(x));
        return *this;
    }
    LongInteger operator-(const LongInteger& x) const {
        string num1 = value, num2 = x.value;
        int diff;
        long max_len = max(num1.length(), num2.length());
        string res;
        fillValues(num1, num2);

        string xStr, yStr;
        if (num1 > num2) {
            xStr = num1;
            yStr = num2;
        } else {
            xStr = num2;
            yStr = num1;
        }
        for (int i = max_len - 1; i >= 0; i--) {
            diff = (xStr[i] - '0') - (yStr[i] - '0');

            if (diff >= 0) {
                res.insert(0, to_string(diff));
            } else {
                int prev = i - 1;
                while (prev >= 0) {
                    xStr[prev] = (base + (xStr[prev] - '0') - 1) % base + '0';

                    if (xStr[prev] != '9') {
                        break;
                    } else {
                        prev-= 1;
                    }
                }
                res.insert(0, std::to_string(diff + base));
            }
        }
        return LongInteger(res.erase(0, std::min(res.find_first_not_of('0'), res.size() - 1)));
    }

    friend istream& operator>>(istream& inp, LongInteger& num) {
        string input;
        inp >> input;
        num.value = input;
        return inp;
    }
    friend ostream& operator<<(ostream& output, LongInteger& num) {
        output << num.value;
        return output;
    }
    static string add_strings(const string& num1, const string& num2) {
        int carry = 0;
        string result;
        size_t max_len = max(num1.length(), num2.length());
        for (size_t i = 0; i < max_len || carry; i++) {
            if (i < num1.length()) carry += num1[num1.length() - 1 - i] - '0';
            if (i < num2.length()) carry += num2[num2.length() - 1 - i] - '0';
            result.push_back(carry % 10 + '0');
            carry /= 10;
        }
        reverse(result.begin(), result.end());
        return result;
    }
    static string subtract_strings(const string& num1, const string& num2) {
        string result;
        int borrow = 0;
        for (int i = 0; i < num1.length(); i++) {
            int sub = (num1[num1.length() - 1 - i] - '0') - borrow;
            if (i < num2.length()) sub -= (num2[num2.length() - 1 - i] - '0');
            if (sub < 0) {
                sub += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.push_back(sub + '0');
        }
        reverse(result.begin(), result.end());
        return result.erase(0, result.find_first_not_of('0')); // Remove leading zeros
    }
    static LongInteger multiply(const string& num1, const string& num2) {
        LongInteger result("0");
        for (int i = 0; i < num2.length(); i++) {
            int digit = num2[num2.length() - 1 - i] - '0';
            string temp = num1;
            for (int j = 0; j < digit; j++) {
                result = LongInteger(add_strings(result.value, temp));
            }
            temp.append(i, '0'); // shift left
            result = LongInteger(add_strings(result.value, temp));
        }
        return result;
    }
    static string shift(const string& num, long zeros) {
        return num + string(zeros, '0');
    }
    static string multiplyByPowerOf10(string& num, long times) {
        for (int k = 0; k < times; k++)
            num.append("0");
        return num;
    }
    static void fillValues(string& num1, string& num2) {
        if (num1.length() < num2.length()) {
            num1 = string(num2.length() - num1.length(), '0') + num1;
        } else if (num2.length() < num1.length()) {
            num2 = string(num1.length() - num2.length(), '0') + num2;
        }
    }

    static string katsarubaMethod(LongInteger& number1, LongInteger& number2)
    {
        if (number1.value.length() == 1 && number2.value.length() == 1) {
            long val1 = number1.value[0] - '0';
            long val2 = number2.value[0] - '0';
            return std::to_string(val1 * val2);
        }

        long num_len = std::max(number1.value.size(), number2.value.size());
        long half = num_len / 2;

        string num1 = number1.value, num2 = number2.value;

        fillValues(num1, num2);

        // Розбиття чисел на половини
        LongInteger a1(num1.substr(0, num1.length() - half));
        LongInteger a0(num1.substr(num1.length() - half));
        LongInteger b1(num2.substr(0, num2.length() - half));
        LongInteger b0(num2.substr(num2.length() - half));

        // Рекурсивні виклики для множення частин
        string m1 = katsarubaMethod(a1, b1);
        string m2 = katsarubaMethod(a0, b0);

        LongInteger a1_a0 = a1 + a0;
        LongInteger b1_b0 = b1 + b0;
        string m3 = katsarubaMethod(a1_a0, b1_b0);

        // Вирахування результату Карацуби
        LongInteger Z1 = LongInteger(m3) - LongInteger(m1) - LongInteger(m2);

        // Зміщення частин (множення на степінь 10)
        m1.append(2 * half, '0');
        Z1.value.append(half, '0');

        // Підсумовування результатів
        LongInteger result = LongInteger(m1) + LongInteger(Z1) + LongInteger(m2);

        return result.value;
    }
    static string toom_cook(LongInteger num1, LongInteger num2) {

        LongInteger res, a0, a1, a2, b0, b1, b2, m1, m2, m0, c1, c2, c3, m01, m02, m12;
        long l = max(num1.value.length(), num2.value.length());

        if (l == 1) {
            return std::to_string((num1.value[0] - '0') * (num2.value[0] - '0'));
        }

        if (l % 3) {
            l += 3 - l % 3;
            while (num1.value.size() < l)
                num1.value.insert(0, "0");
            fillValues(num1.value, num2.value);
        }

        a0 = num1.value.substr(0, l / 3);
        a1 = num1.value.substr(l / 3, (l / 3 > num1.value.length() ? num1.value.length() - l / 3 : l / 3));
        a2 = num1.value.substr(2 * l / 3, (2 * l / 3 > num1.value.length() ? num1.value.length() - 2 * l / 3 : l / 3));
        b0 = num2.value.substr(0, l / 3);
        b1 = num2.value.substr(l / 3, (l / 3 > num2.value.length() ? num2.value.length() - l / 3 : l / 3));
        b2 = num2.value.substr(2 * l / 3, (2 * l / 3 > num2.value.length() ? num2.value.length() - 2 * l / 3 : l / 3));


        m0 = toom_cook(a0, b0);
        m1 = toom_cook(a1, b1);
        m2 = toom_cook(a2, b2);

        c1 = toom_cook((a0 + a1), (b0 + b1));
        c2 = toom_cook((a0 + a2), (b0 + b2));
        c3 = toom_cook((a1 + a2), (b1 + b2));

        m01 = c1 - m0 - m1;
        m02 = c2 - m0 - m2;
        m12 = c3 - m1 - m2;

        m0 = multiplyByPowerOf10(m0.value, 4 * l / 3);
        m01 = multiplyByPowerOf10(m01.value, 3 * l / 3);
        m1 = multiplyByPowerOf10(m1.value, 2 * l / 3);
        m02 = multiplyByPowerOf10(m02.value, 2 * l / 3);
        m12 = multiplyByPowerOf10(m12.value, l / 3);

        res = m0 + m1 + m2 + m01 + m02 + m12;

        return res.value.erase(0, std::min(res.value.find_first_not_of('0'), res.value.size() - 1));
    }
};



