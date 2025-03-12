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
    
    int double_add(int a, int b) {
        // return Math::add(a,b) + Math::add(a,b); //이것도 되는데
        return add(a,b) + add(a,b); //이것도 됨. 왜냐면 같은 namespace 안에서 add() 쓰는거면 Math:: 스킵 가능 
    }
}

int main(){
    std::cout << "Math::add(1,2): " << Math::add(1,2) << std::endl; //3
    std::cout << "Math::substract(1,2): " << Math::substract(1,2) << std::endl; //-1
    std::cout << "double add(1,2): " << Math::double_add(1,2) << std::endl; //3
}
