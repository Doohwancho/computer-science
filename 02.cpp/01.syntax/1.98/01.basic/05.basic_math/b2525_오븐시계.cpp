#include <iostream>

int main() {
    int a, b, c;
    std::cin >> a >> b;
    std::cin >> c;

    a = (a + (c / 60) + ((b + (c % 60)) / 60)) % 24;
    b = (b + (c % 60)) % 60;

    std::cout << a << " " << b << std::endl;

    return 0;
}