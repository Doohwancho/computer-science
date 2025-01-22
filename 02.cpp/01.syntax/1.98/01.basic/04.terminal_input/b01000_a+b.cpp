#include <iostream>

int main() {
    int a,b;
    std::cin >> a >> b;

    auto add = [](int x, int y) { return x + y; };

    auto result = add(a, b);

    std::cout << result << std::endl;

    return 0;
}