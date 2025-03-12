#include <iostream>
#include <memory>


//Q. what is unique pointer?
//A. 소유권 명확하게 하려고. unique_ptr은 한 객체를 한 포인터만 가리킬 수 있음. 이거 쓰면 메모리 관리 더 명확해짐.
//   smart_ptr의 종류라 객체 소멸되면 이 포인터도 같이 소멸됨 

class MyClass {
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

private:
    int value;
};

void processObject(std::unique_ptr<MyClass> obj) {
    obj->printValue();
}

int main() {
    // Create a unique pointer to MyClass
    std::unique_ptr<MyClass> ptr1 = std::make_unique<MyClass>(42);
    ptr1->printValue();

    // Transfer ownership to another unique pointer
    std::unique_ptr<MyClass> ptr2 = std::move(ptr1);
    ptr2->printValue();

    // Pass unique pointer to a function
    processObject(std::move(ptr2));

    // ptr1 and ptr2 are now null

    return 0;
}
