#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <cstdint>

class big_integer {
private:
    std::vector<int> digits;
    bool is_negative;

    void remove_leading_zeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
        if (digits.size() == 1 && digits[0] == 0) {
            is_negative = false;
        }
    }

    bool abs_greater_than(const big_integer& other) const {
        if (digits.size() != other.digits.size()) {
            return digits.size() > other.digits.size();
        }

        for (int i = digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i]) {
                return digits[i] > other.digits[i];
            }
        }
        return false;
    }

    bool abs_equal(const big_integer& other) const {
        if (digits.size() != other.digits.size()) {
            return false;
        }
        for (size_t i = 0; i < digits.size(); ++i) {
            if (digits[i] != other.digits[i]) {
                return false;
            }
        }
        return true;
    }

public:
    big_integer(const std::string& number) : is_negative(false) {
        if (number.empty()) {
            digits.push_back(0);
            return;
        }

        size_t start = 0;
        if (number[0] == '-') {
            is_negative = true;
            start = 1;
        }

        while (start < number.size() && number[start] == '0') {
            ++start;
        }

        if (start == number.size()) {
            digits.push_back(0);
            is_negative = false;
            return;
        }

        for (int i = number.size() - 1; i >= (int)start; --i) {
            if (!std::isdigit(static_cast<unsigned char>(number[i]))) {
                throw std::invalid_argument("Invalid character in number");
            }
            digits.push_back(number[i] - '0');
        }

        remove_leading_zeros();
    }

    big_integer(int number = 0) : is_negative(number < 0) {
        if (number == 0) {
            digits.push_back(0);
            is_negative = false;
        }
        else {
            unsigned int abs_number = (number == INT_MIN) ?
                static_cast<unsigned int>(INT_MAX) + 1 :
                std::abs(number);

            while (abs_number > 0) {
                digits.push_back(abs_number % 10);
                abs_number /= 10;
            }
        }
    }

    big_integer(const big_integer& other) = default;
    big_integer(big_integer&& other) noexcept = default;
    big_integer& operator=(const big_integer& other) = default;
    big_integer& operator=(big_integer&& other) noexcept = default;
    ~big_integer() = default;

    big_integer operator+(const big_integer& other) const {
        if (is_negative != other.is_negative) {
            if (is_negative) {
                big_integer abs_this = *this;
                abs_this.is_negative = false;
                return other - abs_this;
            }
            else {
                big_integer abs_other = other;
                abs_other.is_negative = false;
                return *this - abs_other;
            }
        }

        big_integer result;
        result.is_negative = is_negative;
        result.digits.clear();

        size_t max_size = std::max(digits.size(), other.digits.size());
        int carry = 0;

        for (size_t i = 0; i < max_size || carry; ++i) {
            int sum = carry;
            if (i < digits.size()) sum += digits[i];
            if (i < other.digits.size()) sum += other.digits[i];

            result.digits.push_back(sum % 10);
            carry = sum / 10;
        }

        result.remove_leading_zeros();
        return result;
    }

    big_integer operator-(const big_integer& other) const {
        if (is_negative != other.is_negative) {
            big_integer abs_other = other;
            abs_other.is_negative = !other.is_negative;
            return *this + abs_other;
        }

        bool this_greater = abs_greater_than(other);

        if (abs_equal(other)) {
            return big_integer(0);
        }

        const big_integer* larger = this_greater ? this : &other;
        const big_integer* smaller = this_greater ? &other : this;

        big_integer result;
        result.is_negative = this_greater ? is_negative : !is_negative;
        result.digits.clear();

        int borrow = 0;
        for (size_t i = 0; i < larger->digits.size(); ++i) {
            int diff = larger->digits[i] - borrow;
            if (i < smaller->digits.size()) {
                diff -= smaller->digits[i];
            }

            if (diff < 0) {
                diff += 10;
                borrow = 1;
            }
            else {
                borrow = 0;
            }

            result.digits.push_back(diff);
        }

        result.remove_leading_zeros();
        return result;
    }

    big_integer operator*(const big_integer& other) const {
        if ((digits.size() == 1 && digits[0] == 0) ||
            (other.digits.size() == 1 && other.digits[0] == 0)) {
            return big_integer(0);
        }

        big_integer result;
        result.digits.resize(digits.size() + other.digits.size(), 0);
        result.is_negative = is_negative != other.is_negative;

        for (size_t i = 0; i < digits.size(); ++i) {
            int carry = 0;
            for (size_t j = 0; j < other.digits.size(); ++j) {
                int product = digits[i] * other.digits[j] + result.digits[i + j] + carry;
                result.digits[i + j] = product % 10;
                carry = product / 10;
            }
            if (carry) {
                result.digits[i + other.digits.size()] += carry;
            }
        }

        result.remove_leading_zeros();
        return result;
    }

    big_integer operator*(int multiplier) const {
        if (multiplier == 0) {
            return big_integer(0);
        }

        unsigned int abs_multiplier;
        bool multiplier_negative = (multiplier < 0);

        if (multiplier == INT_MIN) {
            abs_multiplier = static_cast<unsigned int>(INT_MAX) + 1;
        }
        else {
            abs_multiplier = std::abs(multiplier);
        }

        big_integer result;
        result.is_negative = is_negative != multiplier_negative;
        result.digits.clear();

        int carry = 0;
        for (size_t i = 0; i < digits.size() || carry; ++i) {
            int product = carry;
            if (i < digits.size()) {
                product += digits[i] * abs_multiplier;
            }

            result.digits.push_back(product % 10);
            carry = product / 10;
        }

        result.remove_leading_zeros();
        return result;
    }

    bool operator==(const big_integer& other) const {
        if (is_negative != other.is_negative) {
            return false;
        }
        return digits == other.digits;
    }

    bool operator!=(const big_integer& other) const {
        return !(*this == other);
    }

    bool operator<(const big_integer& other) const {
        if (is_negative != other.is_negative) {
            return is_negative;
        }

        if (is_negative) {
            return !abs_greater_than(other) && !abs_equal(other);
        }

        return !abs_greater_than(other) && !abs_equal(other);
    }

    bool operator>(const big_integer& other) const {
        return other < *this;
    }

    bool operator<=(const big_integer& other) const {
        return !(*this > other);
    }

    bool operator>=(const big_integer& other) const {
        return !(*this < other);
    }

    std::string to_string() const {
        if (digits.empty()) {
            return "0";
        }

        std::string result;
        if (is_negative && !(digits.size() == 1 && digits[0] == 0)) {
            result += '-';
        }

        for (int i = digits.size() - 1; i >= 0; --i) {
            result += std::to_string(digits[i]);
        }

        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const big_integer& num) {
        os << num.to_string();
        return os;
    }
};

int main() {
    auto number1 = big_integer("114575");
    auto number2 = big_integer("78524");
    auto result = number1 + number2;
    std::cout << result << std::endl;

    auto number3 = big_integer("999");
    auto number4 = big_integer("1");
    std::cout << number3 + number4 << std::endl;

    auto number5 = big_integer("100");
    auto multiplied = number5 * 5;
    std::cout << multiplied << std::endl;

    auto number6 = big_integer("-50");
    auto multiplied2 = number6 * 3;
    std::cout << multiplied2 << std::endl;

    return 0;
}