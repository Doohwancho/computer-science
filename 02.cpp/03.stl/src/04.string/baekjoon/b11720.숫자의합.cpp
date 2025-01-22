#include <iostream>
#include <string>

int main() {
    int T, total = 0;
    std::string str;
    std::cin >> T >> str;

    for(int i = 0; i < T; i++) {
        total += str[i] - '0';
    }

    std::cout << total << std::endl;

    return 0;
}