#include <iostream>

int main() {
    int T, a, b;
    std::cin >> T;
    for(int i = 0; i < T; i++) {
        std::cin >> a >> b;
        std::cout << "Case #" << i+1 << ": " << a << " + " << b << " = "<< a + b << std::endl;
    }
    return 0;
}