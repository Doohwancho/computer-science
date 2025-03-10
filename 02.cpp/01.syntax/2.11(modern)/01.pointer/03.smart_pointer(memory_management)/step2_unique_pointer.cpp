#include <iostream>
#include <memory>


//what is unique pointer?
//      unique pointer is a pointer that can only be owned by one object.
//      When the object that owns the unique pointer is destroyed, the pointer is also destroyed.

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