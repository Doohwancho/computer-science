#include <vector>

class MyClass {
public:
    MyClass() noexcept {}    

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

//case1)의 연장선상.
int main() {
    std::vector<MyClass> vec;
    // push_back 등이 내부적으로 재할당할 때
    // MyClass의 이동 생성자가 noexcept면 복사 대신 이동 사용
    vec.push_back(MyClass());

    return 0;
}
