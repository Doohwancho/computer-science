#include <iostream>
#include <string>

int main() {
    std::string str;
    int a; 

    std::cin >> str >> a;

    std::cout << str[a-1] << std::endl;

    return 0;
}