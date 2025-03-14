#include <iostream>

//Q. what is const pointer?
//1. const pointer is a pointer that points to a constant value. 
//2. Once initialized, the pointer itself cannot be changed to point to another memory location, 
//3. but the value it points to can be modified
int main(){
    const int num = 10;

    const int* ptr = &num;
    std::cout << "ptr: " << ptr << std::endl; //0x16fd02d08

    // *ptr = 20; //error! - cannot modify the value pointed to by ptr

    //declare non const pointer to value
    int value = 20;
    int* nonConstPtr = &value;

    // assign non const pointer to const pointer
    int* const constPtr = nonConstPtr;
    
    //modify the value through the const pointer
    *constPtr = 30;

    std::cout << *constPtr << std::endl; //30
    
    return 0;
}