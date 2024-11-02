#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <complex>
#include <memory>
#include <random>
using namespace std;
typedef complex<double> complex_double;

class LongInteger {
public:
    string value;
    long int base = 10;
    vector<int> coefficients;

    LongInteger() : value("0") {}
    LongInteger(string x) : value(x) {}
    LongInteger(int x) : value(to_string(x)) {}

    static double PI() {
        return acos(-1);
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
    LongInteger operator/(const LongInteger& other) const {
        return LongInteger(to_string(stoll(value) / stoll(other.value)));
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
    static long long int modPow(long long int number, long long int degree) {
        long long int result = 1;
        while (degree > 0) {
            if (degree % 2 == 1) result = (result * number);
            number = (number * number);
            degree /= 2;
        }
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
    static long long int gcd(long long int a, long long int b) {
        if (b == 0) {
            return a;
        }
        return gcd(b, a % b);
    }

    static long long int mod_power(long long int number, unsigned long long int degree, long long int modul) {
        long long int res = 1;
        number = number % modul;
        while (degree > 0) {
            if (degree & 1) {
                res = (res * number) % modul;
            }
            degree = degree >> 1;
            number = (number * number) % modul;
        }
        return res;
    }
    static long long int linearCongruentialGenerator(long long int seed, long long int a, long long int c, long long int m) {
        return (a * seed + c) % m;
    }

    static long long int randomInRange(long long int n) {
        const long long int a = 48271;
        const long long int c = 0;
        const long long int m = 2147483647;

        static long long int seed = time(nullptr);
        seed = linearCongruentialGenerator(seed, a, c, m);
        return 2 + (seed % (n - 2));
    }
    static void linear_congruent(long long int amount, double rand[]) {
        int a = 74;
        int c = 75;
        long m = pow(2, 16) + 1;
        static int x0 = 1;

        for (int i = 0; i < amount; i++) {
            x0 = (x0 * a + c) % m;
            rand[i] = static_cast<double>(x0) / m;
        }
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
    static string toom_cook(const LongInteger& number1, const LongInteger& number2) {
        string num1 = number1.value, num2 = number2.value;

        fillValues(num1, num2);
        int n = max(number1.value.size(), number2.value.size());

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
    static void fft(vector<complex_double>& arr, bool invert) {
        int n = arr.size();
        if (n == 1) return;

        vector<complex_double> even(n / 2), odd(n / 2);
        for (int i = 0; 2 * i < n; i++) {
            even[i] = arr[2 * i];
            odd[i] = arr[2 * i + 1];
        }

        fft(even, invert);
        fft(odd, invert);

        double angle = 2 * PI() / n * (invert ? -1 : 1);
        complex_double w(1.0, 0.0);
        complex_double wn(cos(angle), sin(angle));
        for (int i = 0; 2 * i < n; i++) {
            arr[i] = even[i] + w * odd[i];
            arr[i + n / 2] = even[i] - w * odd[i];
            if (invert) {
                arr[i] /= 2;
                arr[i + n / 2] /= 2;
            }
            w *= wn;
        }
    }
    static string ShonhageStrassen(const LongInteger& number1, const LongInteger& number2) {
        string num1 = number1.value, num2 = number2.value;
        vector<int> a(num1.size()), b(num2.size());

        // Reverse and convert strings to integer arrays
        for (int i = 0; i < num1.size(); i++)
            a[i] = num1[num1.size() - 1 - i] - '0';
        for (int i = 0; i < num2.size(); i++)
            b[i] = num2[num2.size() - 1 - i] - '0';

        int n = 1;
        while (n < a.size() + b.size()) n <<= 1;
        vector<complex_double> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        fa.resize(n);
        fb.resize(n);

        // Perform FFT on both arrays
        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; i++) {
            fa[i] *= fb[i];
        }
        fft(fa, true);

        // Convert results back to integer format with carry handling
        vector<int> result(n);
        for (int i = 0; i < n; i++) {
            result[i] = round(fa[i].real());
        }

        int carry = 0;
        for (int i = 0; i < n; i++) {
            result[i] += carry;
            carry = result[i] / 10;
            result[i] %= 10;
        }
        while (result.size() > 1 && result.back() == 0)
            result.pop_back();

        string product;
        for (int i = result.size() - 1; i >= 0; i--)
            product += to_string(result[i]);

        return product;
    }
    static string inverseValue(const LongInteger& number, long long int precision) {
        if (number.value == "0") return "0";

        LongInteger z("1");  // Initial approximation
        long long int k = 0;

        while (z.value.length() < precision) {
            LongInteger temp = z * number;
            temp = LongInteger("2") - temp;  // Compute 2 - z * number
            z = z * temp;  // Refine approximation: z = z * (2 - z * number)
            k++;
        }

        return z.value.substr(0, precision);  // Return result to required precision
    }
    static bool LamerTest(LongInteger& num1, long long int it) {
        long long int n = stoll(num1.value);

        // Basic checks for small values and divisibility by 2 or 3
        if (n <= 3) return n > 1;
        if (n % 2 == 0 || n % 3 == 0) return false;

        long long int d = n - 1;
        while (d % 2 == 0) d /= 2;

        for (long long int i = 0; i < it; ++i) {
            long long int a = 2 + rand() % (n - 3);
            long long int x = mod_power(a, d, n);

            if (x == 1 || x == n - 1) continue;

            bool isComposite = true;
            for (long long int j = 0; j < n - 1; ++j) {
                x = (x * x) % n;

                if (x == 1) return false;
                if (x == n - 1) {
                    isComposite = false;
                    break;
                }
            }
            if (isComposite) return false;
        }
        return true;
    }
    static bool Miller_RabinTest(LongInteger& number, long long int it) {
        long long num = stoll(number.value);
        if (num <= 3) return num > 1;
        if (num % 2 == 0 || num % 3 == 0) return false;

        // Розкладання числа на вид n - 1 = 2^r * d
        long long d = num - 1;
        while (d % 2 == 0) {
            d /= 2;
        }
        double rand[1];  // Зберігаємо лише одне значення для кожного виклику
        for (int i = 0; i < it; ++i) {
            // Генеруємо випадкове число та конвертуємо його до діапазону [2, num - 2]
            linear_congruent(1, rand);
            long long a = 2 + static_cast<long long>(rand[0] * (num - 4));

            // Перевірка a^d % num з використанням степеневого зведення
            long long x = mod_power(a, d, num);
            if (x == 1 || x == num - 1) continue;

            bool composite = true;
            for (long long r = d; r < num - 1; r *= 2) {
                x = (x * x) % num;
                if (x == num - 1) {
                    composite = false;
                    break;
                }
            }
            if (composite) return false;
        }
        return true;
    }
    static long long int jacobi(long long int number, long long int a) {
        if (number <= 0 || number % 2 == 0) return 0;
        int j = 1;
        if (a < 0) {
            a = -a;
            if (number % 4 == 3) j = -j;
        }
        while (a != 0) {
            while (a % 2 == 0) {
                a /= 2;
                if (number % 8 == 3 || number % 8 == 5) j = -j;
            }
            swap(a, number);
            if (a % 4 == 3 && number % 4 == 3) j = -j;
            a %= number;
        }
        return number == 1 ? j : 0;
    }
    static bool soloveyShtrassen(LongInteger number, long long int it) {
        long long int num = stoll(number.value);
        if (stoll(number.value) % 2 == 0 && number != 2) return false;
        else if (number == 2) return true;
        else if (number < 2) return false;
        else {
            for (long long int i = 0; i < it; ++i) {
                long long int a = randomInRange(num);
                long long int jac = jacobi(num, a);
                long long int step_val = mod_power(a, (num - 1) / 2, num);

                if (jac == -1) {
                    jac = num - 1;
                }

                if (step_val != jac) {
                    return false;
                }
            }
                return true;
        }
    }
    static bool frobeniusTest(LongInteger number, long long int iterations = 5) {
        if (stoll(number.value) <= 3) return stoll(number.value) > 1;
        if (stoll(number.value) % 2 == 0 || stoll(number.value) % 3 == 0) return false;
        else {
            LongInteger n_minus_1 = number - LongInteger("1");
            LongInteger half_n_minus_1 = stoll(n_minus_1.value) / 2;

            for (int i = 0; i < iterations; ++i) {
                double rand[1];
                linear_congruent(1, rand);
                LongInteger a = LongInteger(to_string(2 + static_cast<int>(rand[0] * (stoll(number.value) - 3))));

                LongInteger x = mod_power(stoll(a.value), stoll(half_n_minus_1.value), stoll(number.value));
                if (x == LongInteger("1")) continue;
                if (x != n_minus_1) return false;
            }
            return true;
        }
    }
    static void output_prime(bool res) {
        if (res) {
            cout << "Is Value Prime: TRUE" << endl;
            return;
        }
        cout << "Is Value Prime: FALSE" << endl;
    }
};
