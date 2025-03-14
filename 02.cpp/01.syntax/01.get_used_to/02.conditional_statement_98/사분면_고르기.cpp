#include <iostream>

int main() {
    int a, b;
    std::cin >> a >> b;
    // ++ -> 1
    // -+ -> 2
    // -- -> 3
    // +- -> 4
    if(a > 0 && b > 0) {
        std::cout << 1 << std::endl;
    } else if(a < 0 && b > 0) {
        std::cout << 2 << std::endl;
    } else if(a < 0 && b < 0) {
        std::cout << 3 << std::endl;
    } else if(a > 0 && b < 0) {
        std::cout << 4 << std::endl;
    }
    return 0;
}