#include <iostream>

//Q. what is function pointer?
// function pointers are pointers that point to functions rather than data.
// They are used to store the address of a function, allowing you to call that function indirectly through the pointer
int add(int a, int b){
    return a + b;
}

int substract(int a, int b){
    return a - b;
}

int main() {
    //declare function pointer
    int(*operationPtr)(int, int);

    std::cout << "operationPtr: " << reinterpret_cast<void*>(operationPtr) << std::endl; // 0x100f90060
    operationPtr = &add;
    std::cout << "operationPtr: " << reinterpret_cast<void*>(operationPtr) << std::endl; // 0x100776e70

    std::cout << "operationPtr: " << operationPtr << std::endl; //1 as true
    int result = operationPtr(1,2);
    std::cout << result << std::endl; //3

    operationPtr = &substract;
    std::cout << "operationPtr: " << reinterpret_cast<void*>(operationPtr) << std::endl; // 0x100776e70

    result = operationPtr(1,2);
    std::cout << result << std::endl; //-1
    std::cout << "operationPtr: " << operationPtr << std::endl; //1 as true
}