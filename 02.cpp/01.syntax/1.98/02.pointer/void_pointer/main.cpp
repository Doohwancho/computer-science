#include <iostream>

/**
 * Q. usecase for void pointer?
 * 
 * 1. commonly used in scenarios where the specific data type of the pointed-to object is not known at compile time
 * 2. where a function needs to work with different data types


 * Q. void pointer의 특징?
 * 1. 여러 타입의 pointer와 함께 사용되는데, return memory address를 여러 타입의 객체에 묶을 수 있다.
 * ex. malloc(), calloc()에서 return type은 void*
 * 약간 generic for pointer?

 * 2. arbitrary data type을 가리킬 수 있다. -> allows callback function to be more flexible
 */
int main() {
    /**
     * static_cast로 void pointer를 integer, float, char pointer로 변환
     * 
     */
    int intValue = 10;
    float floatValue = 3.14;
    char charValue = 'a';

    //declare void pointer
    void* ptr;

    //point to interger
    ptr = &intValue;
    std::cout << "ptr: " << ptr << std::endl; //0x16fd02d08
    // std::cout << "ptr: " << *ptr << std::endl; //error
    std::cout << "ptr: " << *(static_cast<int*>(ptr)) << std::endl;

    //point to float
    ptr = &floatValue;
    std::cout << "ptr: " << ptr << std::endl; //0x16fd02d04
    std::cout << "ptr: " << *(static_cast<float*>(ptr)) << std::endl; //3.14

    //point to char
    ptr = &charValue;
    std::cout << "ptr: " << ptr << std::endl; //0x16fd02d03
    std::cout << "ptr: " << *(static_cast<char*>(ptr)) << std::endl; //'a'

    return 0;
}