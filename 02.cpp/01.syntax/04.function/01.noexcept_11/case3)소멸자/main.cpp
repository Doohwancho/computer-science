// C++11부터 모든 소멸자는 별도 지정이 없으면 암묵적으로 noexcept입니다:
class MyClass {
public:
    ~MyClass() /* 암묵적으로 noexcept */ {
        // 구현
    }
};
