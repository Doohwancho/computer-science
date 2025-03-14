#include <iostream>

// Q. what is std::make_shared?
// A. shared_ptr인데, 더 메모리 해제 측면에서 간편함 

class MyClass {
private:
    int value;

public:
    MyClass(int value) : value(value) {
        std::cout << "Constructor called." << std::endl;
    }

    ~MyClass() {
        std::cout << "Destructor called." << std::endl;
    }

    void printValue() {
        std::cout << "Value: " << value << std::endl;
    }
};

void processObject(std::shared_ptr<MyClass> obj) { 
    obj->printValue();
}

int main() {
    //case1) 
    // C++11 이전 방식 (비추)
    std::shared_ptr<MyClass> ptr(new MyClass(42)); // <-------- delete 또 해줘야 하는데 귀찮음 

    // C++11 make_shared 방식 (추천)
    auto ptr = std::make_shared<MyClass>(42); // <----- delete MyClass 안해줘도 되서 메모리 아낄 수 있음 


    //case2) 
    // 기본 타입 shared_ptr 생성
    auto num_ptr = std::make_shared<int>(42);

    // 벡터 shared_ptr 생성
    auto vec_ptr = std::make_shared<std::vector<int>>(5, 10); // 5개의 10으로 초기화된 벡터

    // 클래스 인스턴스 shared_ptr 생성
    auto obj_ptr = std::make_shared<MyClass>(42);

    // 배열도 가능 (C++20부터)
    auto arr_ptr = std::make_shared<int[]>(10); // 10개 int 배열


    return 0;
}
