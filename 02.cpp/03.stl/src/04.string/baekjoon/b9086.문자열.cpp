#include <iostream>
#include <string>

int main() {
    int T;
    std::string str;

    std::cin >> T;
    for(int i = 0; i < T; i++){
        std::cin >> str; 
        std::cout << str.front() << str.back() << std::endl;
    }
    
    return 0;
}