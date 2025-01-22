#include <iostream>

int main() {
    int T;
    std::cin >> T;
    std::string str = "int";
    for(int i = 0; i < T / 4; i++) {
        //add "long " at the beginning of str
        str = "long " + str;
    }
    std::cout << str;
    return 0;
}