#include <iostream>

//Q. what is namespace?
// 1. namespaces are used to organize code(variable, function, classes) into logical groups 
// 2. prevent naming conflicts
// 3. encapsulation
// 4. simplified access: ex. Math::add(1,2)

namespace Math {
    int add(int a, int b) {
        return a + b;
    }

    int substract(int a, int b) {
        return a - b;
    }
}

int main(){
    std::cout << "Math::add(1,2): " << Math::add(1,2) << std::endl; //3
    std::cout << "Math::substract(1,2): " << Math::substract(1,2) << std::endl; //-1
}