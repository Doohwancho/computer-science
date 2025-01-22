#include <iostream>

int main() {
    //lvalue: variables with persistent memory locations
    int a = 2;  //lvalue
    int b = 10; //lvalue
    int c; //lvalue

    //rvalue: variables with temporary memory locations
    int sum = a + b; //'a+b' is rvalue
    c = a + b; // 'a+b' is rvalue

    std::cout << "sum: " << c << std::endl; //12
    std::cout << "sum: " << sum << std::endl; //12
    

    return 0;
}