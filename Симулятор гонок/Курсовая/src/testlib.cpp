#include <iostream>
extern "C" __declspec(dllexport) void test() {
    std::cout << "Test" << std::endl;
}