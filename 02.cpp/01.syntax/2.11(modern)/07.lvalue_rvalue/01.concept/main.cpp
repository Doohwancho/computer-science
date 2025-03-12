#include <iostream>

//Q. what is lvalue and rvalue?
//A. 98에 개념만 먼저 등장했음
//   lvalue: 주소를 가질 수 있고, 수정 가능.
//   rvalue: 임시 값으로, 대입문의 좌변에 올 수 없음.


int main() {
    //step1) lvalue: variables with persistent memory locations
    int a = 2;  //lvalue
    int b = 10; //lvalue
    int c; //lvalue

    //step2) rvalue: variables with temporary memory locations
    int sum = a + b; //'a+b' is rvalue
    c = a + b; // 'a+b' is rvalue

    std::cout << "sum: " << c << std::endl; //12
    std::cout << "sum: " << sum << std::endl; //12

    return 0;
}
