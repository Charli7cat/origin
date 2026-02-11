#include <iostream>

#define MODE 1

#ifndef MODE
#error "Необходимо определить MODE"
#endif

#if MODE == 0
int main() {
    std::cout << "I work in training mode" << std::endl;
    return 0;
}

#elif MODE == 1
int add(int a, int b) {
    return a + b;
}

int main() {
    std::cout << "I'm working in combat mode" << std::endl;

    int num1, num2;

    std::cout << "Enter the number 1: ";
    std::cin >> num1;

    std::cout << "Enter the number 2: ";
    std::cin >> num2;

    int result = add(num1, num2);
    std::cout << "The result of the addition: " << result << std::endl;

    return 0;
}

#else
int main() {
    std::cout << "Unknown mode. Shutting down" << std::endl;
    return 0;
}
#endif