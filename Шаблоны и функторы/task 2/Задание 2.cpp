#include <iostream>
#include <vector>
#include <utility>

template <typename T>
class table {
private:
    std::vector<std::vector<T>> data;

public:
    table(size_t rows, size_t cols) {
        data.resize(rows);
        for (size_t i = 0; i < rows; ++i) {
            data[i].resize(cols);
        }
    }

    std::vector<T>& operator[](size_t row) {
        return data[row];
    }

    const std::vector<T>& operator[](size_t row) const {
        return data[row];
    }

    std::pair<size_t, size_t> Size() const {
        if (data.empty()) {
            return { 0, 0 };
        }
        return { data.size(), data[0].size() };
    }
};

int main() {
    table<int> test(2, 3);
    test[0][0] = 4;
    std::cout << test[0][0] << std::endl;

    return 0;
}