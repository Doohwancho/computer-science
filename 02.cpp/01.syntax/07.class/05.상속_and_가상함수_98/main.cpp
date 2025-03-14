#include <iostream>

class Animal {
protected: 
    std::string name;

public:
    // 생성자 
    Animal(const std::string& name) : name(name) { //생성자에서 멤버변수 초기화 하는 문법임 (: name(name))
        std::cout << "동물 생성됨 " << name << std::endl;
    }

    // 가상 함수 
    // 가상함수는 "자식 클래스에서 재정의(오버라이딩)할 거다" 라고 컴파일러한테 알려주는거임 
    // 부모 클래스 포인터로 자식 객체 가리킬 때 자식의 함수가 호출되게 해줌
    // 내부적으로는 가상 함수 테이블(vtable)이라는 포인터 배열 만들어서 구현됨 ㅇㅇ 
    virtual void makeSound() {
        std::cout << name << "이(가) 소리를 냄" << std::endl;
    }

    // 소멸자도 virtual로 만들어야 메모리 누수 안 생김 
    // 부모 클래스 포인터로 자식 객체 delete할 때 자식 소멸자 호출 보장함
    // 안 써주면 자식 소멸자 호출 안 되고 부모 소멸자만 호출됨 -> 그러면 자식 객체의 리소스 정리 안 되서 메모리 누수 발생함
    virtual ~Animal() {
        std::cout << "동물 소멸됨: " << name << std::endl;
    }
};

class Dog : public Animal {
public: 
    Dog(const std::string& name) : Animal(name) {
        std::cout << "개 생성됨: " << name << std::endl;
    }

    // overriding
    void makeSound() override {
        std::cout << name << "이(가) 멍멍 짖음" << std::endl; 
    }

    ~Dog() {
        std::cout << "개 소멸됨: " << name << std::endl;
    }
};

int main() {
    // 다형성
    Animal* animal = new Dog("바둑이");
    animal->makeSound();

    delete animal;
    
    return 0;
}
