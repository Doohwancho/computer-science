#include <iostream>
#include <memory>

//case2) what is shared pointer?
//       shared pointer is a pointer that can be shared by multiple objects.
//       When the last shared pointer to an object is destroyed, the object is destroyed.

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

void processObject(std::shared_ptr<MyClass> obj) {
    obj->printValue();
}

int main() {
    // Create a shared pointer to MyClass
    std::shared_ptr<MyClass> ptr1 = std::make_shared<MyClass>(42);
    ptr1->printValue();

    // Share ownership with another shared pointer
    std::shared_ptr<MyClass> ptr2 = ptr1;
    ptr2->printValue();

    // Pass shared pointer to a function
    processObject(ptr1);

    // ptr1 and ptr2 are still valid

    return 0;
}