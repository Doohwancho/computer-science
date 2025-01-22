#include <iostream>

int main() {
    int T, x, y;
    std::cin >> T >> x;

    for(int i = 0; i < T; i++) {
        std::cin >> y;
        if(y < x) {
            std::cout << y << " ";
        }
    }
    return 0;
}