#include <iostream>
#include <string>




class big_integer {
public:
    big_integer() = default;
    big_integer(std::string value) : value_(std::move(value)) {
    }

    big_integer(big_integer&& other) : value_(std::move(other.value_)) {
    }
    big_integer& operator=(big_integer&& other) {
        value_ = std::move(other.value_);
    }

private:
    std::string value_ = "0";

    friend std::ostream& operator<<(std::ostream& os, const big_integer& bi);
    friend big_integer operator+(const big_integer& lhs, const big_integer& rhs);
    friend big_integer operator*(const big_integer& lhs, size_t value);
};

std::ostream& operator<<(std::ostream& os, const big_integer& bi) {
    os << bi.value_;
    return os;
}

char GetSymbolValueForSumTwoChar(char a, char b, bool& add_one_flag) {
    int value = (a - '0') + (b - '0') + static_cast<int>(add_one_flag);
    add_one_flag = value / 10;
    return (value % 10) + '0';
}

char GetSymbolValueForOneChar(char a, bool& add_one_flag) {
    int value = (a - '0') + static_cast<int>(add_one_flag);
    add_one_flag = value / 10;
    return (value % 10) + '0';
}

char GetSymbolValueForProductTwoChar(char a, char b, int& add_carry_value) {
    int value = (a - '0') * (b - '0') + add_carry_value;
    add_carry_value = value / 10;
    return (value % 10) + '0';
}

big_integer operator+(const big_integer& lhs, const big_integer& rhs) {
    std::string result;
    result.reserve(std::max(lhs.value_.size(), rhs.value_.size()) + 1);

    bool add_one_flag = false;

    auto it_lhs = lhs.value_.rbegin();
    auto it_rhs = rhs.value_.rbegin();
    for (; it_lhs != lhs.value_.rend() && it_rhs != rhs.value_.rend(); ++it_lhs, ++it_rhs) {
        result += GetSymbolValueForSumTwoChar(*it_lhs, *it_rhs, add_one_flag);
    }

    if (it_lhs == lhs.value_.rend()) {
        for (; it_rhs != rhs.value_.rend(); ++it_rhs) {
            result += GetSymbolValueForOneChar(*it_rhs, add_one_flag);
        }
    } else if (it_rhs == rhs.value_.rend()) {
        for (; it_lhs != lhs.value_.rend(); ++it_lhs) {
            result += GetSymbolValueForOneChar(*it_lhs, add_one_flag);
        }
    }

    if (add_one_flag) {
        result += '1';
    }

    return std::string(result.rbegin(), result.rend());
}

big_integer operator*(const big_integer& lhs, size_t value) {
    if (value == 0 || lhs.value_ == "0") {
        return big_integer();
    }
    std::string rhs = std::to_string(value);

    std::string result;
    result.reserve(lhs.value_.size() + rhs.size());

    int add_carry_value = 0;
    int m = lhs.value_.size() - 1;
    int n = rhs.size() - 1;

    for (int i = 0; i <= m + n; ++i) {
        for (int j = std::max(0, i - n); j <= std::min(i, m); ++j) {
            add_carry_value += (lhs.value_[m - j] - '0') * (rhs[n - i + j] - '0');
        }
        result += add_carry_value % 10 + '0';
        add_carry_value /= 10;
    }

    return std::string(result.rbegin(), result.rend());
}

int main() {
    auto number1 = big_integer("114575");
    auto number2 = big_integer("78524");
    auto result = number1 + number2;
    std::cout << result << std::endl; // 193099
    auto mult_result = result * 15;
    std::cout << mult_result; // 2896485
    return 0;
}
