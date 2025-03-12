#include <iostream>
#include <memory>

//what is weak pointer?
//weak pointer is a pointer that does not prevent its referent from being destroyed.
//It does this by holding a weak reference to the referent.

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

int main() {
    // Create a shared pointer to MyClass
    std::shared_ptr<MyClass> sharedPtr = std::make_shared<MyClass>(42);

    // Create a weak pointer to the shared object
    std::weak_ptr<MyClass> weakPtr = sharedPtr;

    // Check if the weak pointer is valid
    if (std::shared_ptr<MyClass> obj = weakPtr.lock()) {
        obj->printValue();
    } else {
        std::cout << "Weak pointer is expired." << std::endl;
    }

    // Reset the shared pointer
    sharedPtr.reset();

    // Check if the weak pointer is valid again
    if (std::shared_ptr<MyClass> obj = weakPtr.lock()) {
        obj->printValue();
    } else {
        std::cout << "Weak pointer is expired." << std::endl;
    }

    return 0;
}