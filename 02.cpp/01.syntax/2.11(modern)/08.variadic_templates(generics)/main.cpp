
#include <iostream>

template <typename... Args>

void printValues(Args... args) {
    ((std::cout << args << " "), ...);
}

int main() {
    printValues(1, 3.14, "hello");  // Output: 1 3.14 hello
    return 0;
}