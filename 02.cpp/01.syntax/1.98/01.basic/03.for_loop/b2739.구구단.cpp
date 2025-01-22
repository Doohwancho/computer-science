#include <iostream>

int main () {
    int a; 
    std::cin >> a; 

    for(int i = 0; i < 9; i++) {
        std::cout << a << " * " << i+1 << " = " << a * (i+1) << std::endl;
    }

    return 0;
}