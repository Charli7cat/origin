#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <string>

// Основная шаблонная функция для печати контейнера
template<typename Container>
void print_container(const Container& container) {
    auto it = container.begin();

    if (it != container.end()) {
        std::cout << *it;
        ++it;
    }

    for (; it != container.end(); ++it) {
        std::cout << ", " << *it;
    }
    std::cout << std::endl;
}

template<typename T>
void print_container(const std::set<T>& container) {
    auto it = container.begin();

    if (it != container.end()) {
        std::cout << *it;
        ++it;
    }

    for (; it != container.end(); ++it) {
        std::cout << ", " << *it;
    }
    std::cout << std::endl;
}

int main() {
    std::set<std::string> test_set = { "one", "two", "three", "four" };
    std::cout << "std::set: ";
    print_container(test_set);

    std::cout << "\nWhy is this order?\n";
    std::cout << "std::set stores the items in sorted order.\n";
    std::cout << "Strings are sorted lexicographically:\n";
    std::cout << "'four' < 'one' < 'three' < 'two'\n\n";

    std::list<std::string> test_list = { "one", "two", "three", "four" };
    std::cout << "std::list: ";
    print_container(test_list);

    std::vector<std::string> test_vector = { "one", "two", "three", "four" };
    std::cout << "std::vector: ";
    print_container(test_vector);

    return 0;
}