#include <iostream>

int main() {
    /**
     * case1 - basic
     */
    int num = 42;
    int* ptr = &num;

    std::cout << "num: " << num << std::endl; //42
    std::cout << "ptr: " << ptr << std::endl; //0x7ffeefbff76c
    std::cout << "*ptr: " << *ptr << std::endl; //42

    num = 100;
    std::cout << "num: " << num << std::endl; //100
    std::cout << "ptr: " << ptr << std::endl; //0x7ffeefbff76c
    std::cout << "*ptr: " << *ptr << std::endl; //100

    *ptr = 200; //dereference ptr
    std::cout << "num: " << num << std::endl; //200
    std::cout << "ptr: " << ptr << std::endl; //0x7ffeefbff76c
    std::cout << "*ptr: " << *ptr << std::endl; //200

    std::cout << "----------- end of case 1 ------------" << std::endl;

    /**
     * case2 - can insert null
     */
    ptr = NULL; //ptr, 주소에 null값 넣을 수 있다. reference는 null값 못넣는데.
    std::cout << "----------- end of case 2 ------------" << std::endl;


    /**
     * case3 - pointer arithemtic
     */
     int arr[] = {10, 20, 30, 40, 50};
     int* ptrArr = arr;

     std::cout << ptrArr << std::endl; //주소
     std::cout << ptrArr + 1 << std::endl; //주소

     std::cout << *ptrArr << std::endl; //10
     std::cout << *ptrArr + 1 << std::endl; //11
     std::cout << *(ptrArr + 1) << std::endl; //20

     for(int i = 0; i < 5; i++) {
         std::cout << *(ptrArr + i) << std::endl;
     }
    std::cout << "----------- end of case 3 ------------" << std::endl;
}