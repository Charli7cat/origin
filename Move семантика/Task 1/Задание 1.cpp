#include <vector>
#include <string>
#include <algorithm>
#include <utility>

template<typename T>
void move_vectors(std::vector<T>& source, std::vector<T>& destination) {
    destination = std::move(source);
}

template<typename T>
void move_vectors_append(std::vector<T>& source, std::vector<T>& destination) {
    destination.insert(
        destination.end(),
        std::make_move_iterator(source.begin()),
        std::make_move_iterator(source.end())
    );
    source.clear();
}

template<typename T>
void move_vectors_manual(std::vector<T>& source, std::vector<T>& destination) {
    destination.reserve(destination.size() + source.size());

    for (auto& item : source) {
        destination.push_back(std::move(item));
    }

    source.clear();
}

int main() {
    std::vector<std::string> one = { "test_string1", "test_string2" };
    std::vector<std::string> two;

    move_vectors(one, two);


    return 0;
}