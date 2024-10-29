#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <ctime>
#include <memory>
using namespace std;

class LongInteger {
public:
    string value;
    long int base = 10;
    const double PI = acos(-1);
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
    bool operator==(const LongInteger& x) {
        return value == x.value;
    }
    bool operator!=(const LongInteger& x) {
        return !(value == x.value);
    }
    LongInteger& operator=(LongInteger x) {
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
    LongInteger operator*(const LongInteger& other) const {
        return LongInteger(katsarubaMethod(*this, other));
    }

    LongInteger operator*(int n) const {
        return *this * LongInteger(to_string(n));
    }
    bool operator>(const LongInteger& other) {
        if (value.size() != other.value.size()) {
            return value.size() > other.value.size();
        }
        for (int i = int(value.size()) - 1; i >= 0; --i) {
            if (value[i] > other.value[i]) {
                return true;
            } else if (value[i] < other.value[i]) {
                return false;
            }
        }
        return false;
    }
    bool operator<(const LongInteger& other) {
        return !(operator>=(other));
    }
    bool operator<=(const LongInteger& other) {
        return !(operator>(other));
    }

    bool operator>=(const LongInteger& other) {
        return (operator>(other) || operator==(other));
    }
    LongInteger operator%(const LongInteger& other) const {
        if (other.value == "0") {
            return LongInteger("0");
        }

        LongInteger dividend(value);
        LongInteger divisor(other.value);

        bool negativeResult = (dividend.value[0] == '-' && divisor.value[0] != '-') || (dividend.value[0] != '-' && divisor.value[0] == '-');
        dividend.value.erase(0, std::min(dividend.value.find_first_not_of('-'), dividend.value.size() - 1));
        divisor.value.erase(0, std::min(divisor.value.find_first_not_of('-'), divisor.value.size() - 1));

        LongInteger currentDividend;
        for (size_t i = 0; i < dividend.value.size(); ++i) {
            currentDividend = currentDividend * 10 + LongInteger(dividend.value[i] - '0');

            while (currentDividend >= divisor) {
                currentDividend = currentDividend - divisor;
            }
        }
        if (negativeResult) {
            currentDividend.value.insert(0, "-");
        }

        return currentDividend;
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
    static string add(const string &num1, const string &num2) {
        string result;
        int carry = 0;
        int i = num1.size() - 1, j = num2.size() - 1;

        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            if (i >= 0) sum += num1[i--] - '0';
            if (j >= 0) sum += num2[j--] - '0';

            carry = sum / 10;
            sum %= 10;

            result += to_string(sum);
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
    string divide_by_int(int divisor) {
        string result;
        int carry = 0;

        for (char digit : value) {
            carry = carry * 10 + (digit - '0');
            result.push_back((carry / divisor) + '0');
            carry %= divisor;
        }

        // Видаляємо провідні нулі
        result.erase(0, min(result.find_first_not_of('0'), result.size() - 1));

        return result;
    }
    static string mult(LongInteger num1, LongInteger num2) {
        if (num2.value == "0") {
            return 0;
        }


        LongInteger dividend(num1.value);
        LongInteger divisor(num2.value);

        bool negativeResult = (dividend.value[0] == '-' && divisor.value[0] != '-') || (dividend.value[0] != '-' && divisor.value[0] == '-');
        dividend.value.erase(0, std::min(dividend.value.find_first_not_of('-'), dividend.value.size() - 1));
        divisor.value.erase(0, std::min(divisor.value.find_first_not_of('-'), divisor.value.size() - 1));

        LongInteger currentDividend;
        for (size_t i = 0; i < dividend.value.size(); ++i) {
            currentDividend = currentDividend * 10 + LongInteger(dividend.value[i] - '0');

            while (currentDividend >= divisor) {
                currentDividend = currentDividend - divisor;
            }
        }
        if (negativeResult) {
            currentDividend.value.insert(0, "-");
        }

        return currentDividend.value;
    }
    static void fillValues(string& num1, string& num2) {
        if (num1.length() < num2.length()) {
            num1 = string(num2.length() - num1.length(), '0') + num1;
        } else if (num2.length() < num1.length()) {
            num2 = string(num1.length() - num2.length(), '0') + num2;
        }
    }

    static string katsarubaMethod(const LongInteger& number1, const LongInteger& number2)
    {
        LongInteger numb1 = number1, numb2 = number2;
        if (numb1.value.length() == 1 && numb2.value.length() == 1) {
            long val1 = numb1.value[0] - '0';
            long val2 = numb2.value[0] - '0';
            return std::to_string(val1 * val2);
        }

        long num_len = max(numb1.value.size(), numb2.value.size());
        long half = num_len / 2;

        string num1 = numb1.value, num2 = numb2.value;

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
    static string toom_cook(string x, string y) {
        string num1 = x, num2 = y;
        fillValues(num1, num2);
        int n = max(x.size(), y.size());

        if (n <= 4) {
            return to_string(stoll(num1) * stoll(num2));
        }

        int m = (n + 2) / 3;

        string x0 = num1.substr(0, num1.size() - m);
        string x1 = num1.substr(num1.size() - m);
        string y0 = num2.substr(0, num2.size() - m);
        string y1 = num2.substr(num2.size() - m);

        string z0 = toom_cook(x0, y0);
        string z2 = toom_cook(x1, y1);
        string z1 = toom_cook(add(x0, x1), add(y0, y1));
        z1 = subtract_strings(z1, z0);
        z1 = subtract_strings(z1, z2);

        for (int i = 0; i < 2 * m; ++i) {
            z0 += "0";
        }

        for (int i = 0; i < m; ++i) {
            z1 += "0";
        }

        string result = add(add(z0, z1), z2);
        return result;
    }

    static string shenhage(LongInteger& number1, LongInteger& number2) {
        LongInteger divid = LongInteger("123123123123123123123123123132312312312312312312312312312312312312312312312312312312312312312312311231239999999999999999999999999999999999999999999999999");
        LongInteger res = LongInteger(katsarubaMethod(number1, number2)) % divid;
        return res.value;
    }
};



