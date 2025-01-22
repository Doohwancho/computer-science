#include <iostream>

int main() {
    int expected, T, price, quantity, total = 0;
    std::cin >> expected >> T;
    for(int i = 0; i < T; i++){
        std::cin >> price >> quantity;
        total += price * quantity;
    }

    std::cout << (expected == total ? "Yes" : "No") << std::endl;
    return 0;
}