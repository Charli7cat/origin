#include <iostream>

class Fraction
{
private:
    int numerator_;
    int denominator_;

    int gcd(int a, int b) const {
        a = abs(a);
        b = abs(b);
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    void reduce() {
        int gcd_value = gcd(numerator_, denominator_);
        numerator_ /= gcd_value;
        denominator_ /= gcd_value;

        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
    }

public:
    Fraction(int numerator, int denominator)
    {
        numerator_ = numerator;
        denominator_ = denominator;
        reduce();
    }

    int getNumerator() const { return numerator_; }
    int getDenominator() const { return denominator_; }

    bool operator==(const Fraction& other) const {
        return (static_cast<long long>(numerator_) * other.denominator_) ==
            (static_cast<long long>(other.numerator_) * denominator_);
    }

    bool operator!=(const Fraction& other) const {
        return !(*this == other);
    }

    bool operator<(const Fraction& other) const {
        return (static_cast<long long>(numerator_) * other.denominator_) <
            (static_cast<long long>(other.numerator_) * denominator_);
    }

    bool operator>(const Fraction& other) const {
        return other < *this;
    }

    bool operator<=(const Fraction& other) const {
        return !(other < *this);
    }

    bool operator>=(const Fraction& other) const {
        return !(*this < other);
    }


    Fraction operator+(const Fraction& other) const {
        int new_numerator = numerator_ * other.denominator_ + other.numerator_ * denominator_;
        int new_denominator = denominator_ * other.denominator_;
        return Fraction(new_numerator, new_denominator);
    }

    Fraction operator-(const Fraction& other) const {
        int new_numerator = numerator_ * other.denominator_ - other.numerator_ * denominator_;
        int new_denominator = denominator_ * other.denominator_;
        return Fraction(new_numerator, new_denominator);
    }

    Fraction operator*(const Fraction& other) const {
        int new_numerator = numerator_ * other.numerator_;
        int new_denominator = denominator_ * other.denominator_;
        return Fraction(new_numerator, new_denominator);
    }

    Fraction operator/(const Fraction& other) const {
        int new_numerator = numerator_ * other.denominator_;
        int new_denominator = denominator_ * other.numerator_;
        return Fraction(new_numerator, new_denominator);
    }

    Fraction operator-() const {
        return Fraction(-numerator_, denominator_);
    }

    Fraction& operator++() {
        numerator_ += denominator_;
        reduce();
        return *this;
    }

    Fraction operator++(int) {
        Fraction temp = *this;
        numerator_ += denominator_;
        reduce();
        return temp;
    }

    Fraction& operator--() {
        numerator_ -= denominator_;
        reduce();
        return *this;
    }

    Fraction operator--(int) {
        Fraction temp = *this;
        numerator_ -= denominator_;
        reduce();
        return temp;
    }

    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        os << f.numerator_ << '/' << f.denominator_;
        return os;
    }
};

int main()
{
    int num1, den1, num2, den2;

    std::cout << "Enter the denominator of the fraction 1: ";
    std::cin >> num1;
    std::cout << "Enter the denominator of the fraction 1: ";
    std::cin >> den1;

    std::cout << "Enter the denominator of the fraction 2: ";
    std::cin >> num2;
    std::cout << "Enter the denominator of the fraction 2: ";
    std::cin >> den2;

    Fraction f1(num1, den1);
    Fraction f2(num2, den2);

    std::cout << "\n--- Arithmetic operations ---\n";
    std::cout << f1 << " + " << f2 << " = " << (f1 + f2) << '\n';
    std::cout << f1 << " - " << f2 << " = " << (f1 - f2) << '\n';
    std::cout << f1 << " * " << f2 << " = " << (f1 * f2) << '\n';
    std::cout << f1 << " / " << f2 << " = " << (f1 / f2) << '\n';
    std::cout << "-" << f1 << " = " << (-f1) << '\n';

    std::cout << "\n--- Demonstration of the decrement ---\n";
    Fraction f3 = f1;
    std::cout << "Prefix increment: ++" << f1 << " * " << f2 << " = " << (++f3 * f2) << '\n';
    std::cout << "The fraction value after the operation: " << f3 << '\n';

    f3 = f1;
    std::cout << "Postfix increment: " << f1 << "++ * " << f2 << " = " << (f3++ * f2) << '\n';
    std::cout << "The fraction value after the operation: " << f3 << '\n';

    std::cout << "\n--- Demonstration of the decrement ---\n";
    f3 = f1;
    std::cout << "Prefix decrement: --" << f1 << " * " << f2 << " = " << (--f3 * f2) << '\n';
    std::cout << "The fraction value after the operation: " << f3 << '\n';

    f3 = f1;
    std::cout << "Postfix decrement: " << f1 << "-- * " << f2 << " = " << (f3-- * f2) << '\n';
    std::cout << "The fraction value after the operation: " << f3 << '\n';

    return 0;
}