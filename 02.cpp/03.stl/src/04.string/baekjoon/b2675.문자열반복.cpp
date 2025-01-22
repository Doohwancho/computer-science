#include <iostream>
#include <string>

int main() {
    int T, a;
    std::string str;

    std::cin >> T;
    for(int i = 0; i < T; i++){
        std::cin >> a;
        std::cin >> str;
        for(int j = 0; j < (int)str.length(); j++) {
            for(int k = 0; k < a; k++) {
                std::cout << str[j];
            }
        }
        std::cout << std::endl;
    }

    return 0;
}