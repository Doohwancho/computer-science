#include <iostream>

int main() {
    int a = 10;
    int b = 20;
    int c = 30;

    int* ptrArray[] = {&a, &b, &c};

    std::cout << "address of ptrArray: " << ptrArray << std::endl; //배열 첫 주소랑 배열 첫번째 element 주소랑 다르다. 이건 걍 arr[int]가 아닌 배열포인터라서 그런 듯?
    for(int i = 0; i < 3; i++){
        std::cout << "ptrArray[" << i << "]: " << ptrArray[i] << std::endl; //주소
        std::cout << "*ptrArray[" << i << "]: " << *ptrArray[i] << std::endl; //10,20,30
    }

    int** ptrToPtrArray = ptrArray;

    std::cout << "address of ptrToPtrArray: " << ptrToPtrArray << std::endl; //0x16d3b6d00로, ptrArray와 같다.
    std::cout << "address of ptrToPtrArray: " << *ptrToPtrArray << std::endl; //첫 element의 주솟값을 찾아서 출력한다.
    std::cout << "address of ptrToPtrArray: " << **ptrToPtrArray << std::endl; //첫 element의 값을 출력한다.
    for(int i = 0; i < 3; i++){
        std::cout << **(ptrToPtrArray + i) << std::endl; //10,20,30
    }

    return 0;
}