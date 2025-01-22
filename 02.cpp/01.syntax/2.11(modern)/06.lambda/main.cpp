#include <iostream>

int main() {
    auto square = [](int x) { return x * x; };

    int result = square(10);

    std::cout << result << std::endl;

    return 0;
}