#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string str1, str2;
    int a, b;
    std::cin >> str1 >> str2;

    std::reverse(str1.begin(), str1.end()); //string 뒤집는 함수
    std::reverse(str2.begin(), str2.end());

    a = std::stoi(str1); //string -> int. ((int)str1으로 변환 안되더라)
    b = std::stoi(str2);

    if(a > b) {
        std::cout << a << std::endl;
    } else {
        std::cout << b << std::endl;
    }

    return 0;
}