#include <iostream>

//Q. what is reference?
//A. A reference is an alias or an alternative name for an existing object.
//   When you pass an object by reference, you are essentially passing the object itself, not a pointer to it. 
//   This allows the function to directly access and work with the original object without the need for dereferencing

void doubleValue(int& num) { //reference를 파라미터로 받는다
    num *= 2;
}

int main() {
    //case1)
    int num = 42;
    int& ref = num;
    std::cout << "num: " << num << std::endl; //42
    std::cout << "ref: " << ref << std::endl; //42

    //case2)
    num = 100;
    std::cout << "num: " << num << std::endl; //100
    std::cout << "ref: " << ref << std::endl; //100

    ref = 200;
    std::cout << "num: " << num << std::endl; //200
    std::cout << "ref: " << ref << std::endl; //200

    //case3) 
    doubleValue(ref); //reference를 넘겨준다.
    std::cout << "num: " << num << std::endl; //400
    std::cout << "ref: " << ref << std::endl; //400
}
