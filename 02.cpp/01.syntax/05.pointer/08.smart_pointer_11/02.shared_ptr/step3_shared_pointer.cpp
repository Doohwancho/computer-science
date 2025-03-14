#include <iostream>
#include <memory>

//Q. what is shared pointer?
//A  shared pointer is a pointer that can be shared by multiple objects.
//   When the last shared pointer to an object is destroyed, the object is destroyed.

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
    // Create a shared pointer to MyClass
    // std::shared_ptr<MyClass> ptr1 = std::make_shared<MyClass>(42); //Q. what is std::make_shared? 
                                                                      //    A. 아래 파일에 나옴 
    std::shared_ptr<MyClass> ptr1(new MyClass(42));

    ptr1->printValue(); //42

    // Share ownership with another shared pointer
    std::shared_ptr<MyClass> ptr2 = ptr1;
    ptr2->printValue(); //42

    ptr2 = std::make_shared<MyClass>(10);

    // Pass shared pointer to a function
    ptr1->printValue(); //42        <---------- ptr2에 새로운 값이 할당되도 ptr1값이 안바뀌네?
    ptr2->printValue(); //10 

    return 0;
}
