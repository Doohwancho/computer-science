#include <iostream>

// 이동 연산은 대부분 예외를 던지지 않으며, noexcept로 표시하면 최적화 기회가 생깁니다:
class MyClass {
public:
    // 이동 생성자
    MyClass(MyClass&& other) noexcept 
        : data_(std::move(other.data_)) {}
    
    // 이동 대입 연산자
    MyClass& operator=(MyClass&& other) noexcept {
        data_ = std::move(other.data_);
        return *this;
    }
private:
    std::string data_;
};
