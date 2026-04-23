#include <iostream>
#include <stdexcept>
#include <algorithm>

class smart_array {
private:
    int* data;
    size_t size;
    size_t capacity;

public:
    smart_array(size_t initial_capacity)
        : data(nullptr), size(0), capacity(initial_capacity) {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
        data = new int[capacity];
    }

    ~smart_array() {
        delete[] data;
    }

    smart_array(const smart_array& other)
        : data(nullptr), size(other.size), capacity(other.capacity) {
        data = new int[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    smart_array& operator=(const smart_array& other) {
        if (this != &other) {
            smart_array temp(other);

            std::swap(data, temp.data);
            std::swap(size, temp.size);
            std::swap(capacity, temp.capacity);
        }
        return *this;
    }

    void add_element(int value) {
        if (size >= capacity) {
            size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
            resize(new_capacity);
        }
        data[size++] = value;
    }

    int get_element(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    size_t get_size() const {
        return size;
    }

    size_t get_capacity() const {
        return capacity;
    }

    void print() const {
        std::cout << "[";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

private:
    void resize(size_t new_capacity) {
        int* new_data = new int[new_capacity];

        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }

        delete[] data;

        data = new_data;
        capacity = new_capacity;
    }
};

int main() {
    try {
        smart_array arr(5);
        arr.add_element(1);
        arr.add_element(4);
        arr.add_element(155);

        std::cout << "arr before assignment: ";
        arr.print();

        smart_array new_array(2);
        new_array.add_element(44);
        new_array.add_element(34);

        std::cout << "new_array: ";
        new_array.print();

        arr = new_array;

        std::cout << "arr after assignment: ";
        arr.print();

        new_array.add_element(99);
        std::cout << "new_array after modification: ";
        new_array.print();
        std::cout << "arr remains unchanged: ";
        arr.print();

        std::cout << "arr.get_element(1): " << arr.get_element(1) << std::endl;
    }
    catch (const std::exception& ex) {
        std::cout << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}