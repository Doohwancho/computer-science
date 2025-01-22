#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string str1, str2;
    std::cin >> str1;
    str2.assign(str1);
    std::reverse(str1.begin(), str1.end());

    if(str1 == str2) {
        std::cout << 1 << std::endl;
    } else {
        std::cout << 0 << std::endl;
    }
    return 0;
}