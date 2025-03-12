#include <iostream>

class MyClass {
public:
    // explicit 없는 생성자
    MyClass(int x) { std::cout << "정수로 객체 생성: " << x << std::endl; }
};

class SafeClass {
public:
    // explicit 있는 생성자
    explicit SafeClass(int x) { std::cout << "정수로 객체 생성: " << x << std::endl; }
};

void testFunc(MyClass obj) { /* 뭔가 함 */ }
void testSafeFunc(SafeClass obj) { /* 뭔가 함 */ }

int main() {
    // explicit 없으면 이런 개쓰레기 코드가 컴파일됨 ㄹㅇ
    testFunc(42);  // 컴파일러가 MyClass(42)로 암시적 변환해버림
    
    // explicit 있으면 이런 개삽질 못함
    testSafeFunc(42);  // 컴파일 에러! 명시적으로 SafeClass(42) 해야함
    
    // 이렇게 명시적으로 해야 됨
    testSafeFunc(SafeClass(42));  // OK
}
