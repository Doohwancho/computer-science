#include <iostream>
#include <memory>

class Resource {
public:
    void doSomething() {
        std::cout << "Resource::doSomething() called" << std::endl;
    }
};

// 커스텀 스마트 포인터 클래스
template <typename T>
class SmartPtr {
private:
    T* ptr_;
    
public:
    // 생성자
    explicit SmartPtr(T* ptr = nullptr) : ptr_(ptr) {}
    
    // 소멸자
    ~SmartPtr() {
        delete ptr_;
    }
    
    // 복사 금지 (unique_ptr 비슷하게)
    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;
    
    // 이동 생성자
    SmartPtr(SmartPtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }
    
    // 이동 대입 연산자
    SmartPtr& operator=(SmartPtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }
    
    // -> 연산자 오버로딩 (멤버 접근용)
    T* operator->() const {
        if (!ptr_) {
            throw std::runtime_error("Null pointer access");
        }
        return ptr_;
    }
    
    // * 연산자 오버로딩 (역참조용)
    T& operator*() const {
        if (!ptr_) {
            throw std::runtime_error("Null pointer dereference");
        }
        return *ptr_;
    }
    
    // 포인터 체크 연산자
    explicit operator bool() const {
        return ptr_ != nullptr;
    }
};

int main() {
    // 커스텀 스마트 포인터 사용
    SmartPtr<Resource> res1(new Resource());
    
    // -> 연산자 사용
    res1->doSomething();
    
    // * 연산자 사용
    Resource& ref = *res1;
    ref.doSomething();
    
    // bool 변환 연산자 사용
    if (res1) {
        std::cout << "res1 is not null" << std::endl;
    }
    
    // 이동 생성
    SmartPtr<Resource> res2(std::move(res1));
    
    // res1은 이제 nullptr
    if (!res1) {
        std::cout << "res1 is now null after move" << std::endl;
    }
    
    // res2는 유효함
    res2->doSomething();
    
    return 0;
}
