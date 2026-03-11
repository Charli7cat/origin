#include <cassert>
#include <iostream>
#include <stdexcept>

void testEmptyArray() {
    std::cout << "Testing empty array operations..." << std::endl;

    ExtArray<int> empty1({});
    assert(empty1.size() == 0);

    ExtArray<int> empty2(0);
    assert(empty2.size() == 0);

    // Тест метода size()
    assert(empty1.size() == 0);
    assert(empty2.size() == 0);

    try {
        double mean_result = empty1.mean();
        assert(mean_result == 0.0);
    }
    catch (const std::exception& e) {
        std::cout << "  mean() throws exception: " << e.what() << std::endl;
    }

    try {
        double median_result = empty1.median();
        assert(median_result == 0.0);
    }
    catch (const std::exception& e) {
        std::cout << "  median() throws exception: " << e.what() << std::endl;
    }

    try {
        auto mode_result = empty1.mode();
        assert(mode_result.second == 0);
        std::cout << "  mode() returns: (" << mode_result.first
            << ", " << mode_result.second << ")" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "  mode() throws exception: " << e.what() << std::endl;
    }

    std::cout << "Empty array tests completed." << std::endl;
    std::cout << std::string(50, '-') << std::endl;
}

void testSingleElementArray() {
    std::cout << "Testing single element array operations..." << std::endl;

    ExtArray<int> arr({ 42 });
    assert(arr.size() == 1);
    assert(arr.mean() == 42.0);
    assert(arr.median() == 42.0);
    auto mode_result = arr.mode();
    assert(mode_result.first == 42);
    assert(mode_result.second == 1);

    std::cout << "Single element array tests passed." << std::endl;
    std::cout << std::string(50, '-') << std::endl;
}

void testMultiElementArray() {
    std::cout << "Testing multi-element array operations..." << std::endl;

    ExtArray<int> arr({ 1, 2, 3, 4, 5 });
    assert(arr.size() == 5);

    assert(arr[0] == 1);

    assert(arr[4] == 5);

    assert(arr.mean() == 3.0);

    assert(arr.median() == 3.0);

    auto mode_result = arr.mode();
    assert(mode_result.second == 1);

    std::cout << "Multi-element array tests passed." << std::endl;
    std::cout << std::string(50, '-') << std::endl;
}

void testArrayWithDuplicates() {
    std::cout << "Testing array with duplicates..." << std::endl;

    ExtArray<int> arr({ 1, 2, 2, 3, 3, 3, 4, 4, 4, 4 });
    assert(arr.size() == 10);

    double expected_mean = (1 + 2 + 2 + 3 + 3 + 3 + 4 + 4 + 4 + 4) / 10.0;
    assert(arr.mean() == expected_mean);

    assert(arr.median() == 3.0);

    auto mode_result = arr.mode();
    assert(mode_result.first == 4);
    assert(mode_result.second == 4);

    std::cout << "Array with duplicates tests passed." << std::endl;
    std::cout << std::string(50, '-') << std::endl;
}

void testEvenSizeArray() {
    std::cout << "Testing even size array operations..." << std::endl;

    ExtArray<int> arr({ 1, 2, 3, 4 });
    assert(arr.size() == 4);

    assert(arr.median() == 2.5);

    std::cout << "Even size array tests passed." << std::endl;
    std::cout << std::string(50, '-') << std::endl;
}

void testOddSizeArray() {
    std::cout << "Testing odd size array operations..." << std::endl;

    ExtArray<int> arr({ 1, 2, 3, 4, 5 });
    assert(arr.size() == 5);

    assert(arr.median() == 3.0);

    std::cout << "Odd size array tests passed." << std::endl;
    std::cout << std::string(50, '-') << std::endl;
}

int main() {
    std::cout << "Starting ExtArray tests..." << std::endl;
    std::cout << std::string(50, '=') << std::endl;

    testEmptyArray();

    testSingleElementArray();
    testMultiElementArray();
    testArrayWithDuplicates();
    testEvenSizeArray();
    testOddSizeArray();

    std::cout << "All tests completed!" << std::endl;

    return 0;
}