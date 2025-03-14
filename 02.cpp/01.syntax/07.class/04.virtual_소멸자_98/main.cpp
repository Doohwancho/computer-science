#include <iostream>

class Base {
public:
    // case1) 일반 소멸자 (virtual 아님)
    ~Base() { 
    //case2) virtual 소멸자 
    // virtual ~Base() {
        std::cout << "Base 소멸자 호출" << std::endl;
    }
    
    // 가상 함수 (동적 바인딩)
    virtual void print() {
        std::cout << "Base::print()" << std::endl;
    }
};

class Derived : public Base {
private:
    int* data;
    
public:
    Derived() {
        data = new int[100];  // 동적 메모리 할당
        std::cout << "Derived 생성자: 메모리 할당됨" << std::endl;
    }
    
    // 소멸자
    ~Derived() {
        delete[] data;  // 메모리 해제
        std::cout << "Derived 소멸자: 메모리 해제됨" << std::endl;
    }
    
    void print() override {
        std::cout << "Derived::print()" << std::endl;
    }
};

int main() {
    // 문제의 코드: Base 포인터로 Derived 객체 가리킴
    Base* ptr = new Derived();
    
    ptr->print();  // Derived::print() 호출 (virtual 함수라 동적 바인딩)
    
    delete ptr;
    // 여기서 문제 발생: Base 소멸자만 호출하고 Derived 소멸자는 호출 안 됨
    // 결과: data 배열의 메모리 누수 발생!

    std::cout << "------------------------------" << std::endl;
    
    // 수정된 Base 클래스에 virtual 소멸자 있으면:
    // virtual ~Base() { std::cout << "Base 소멸자 호출" << std::endl; }
    // 이 경우 delete ptr할 때 두 소멸자 모두 제대로 호출됨
}
