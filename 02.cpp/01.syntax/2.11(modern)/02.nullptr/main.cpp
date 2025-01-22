#include <iostream>

void printValue(int* ptr) {
    if (ptr != nullptr) {
        std::cout << "Value: " << *ptr << std::endl;
    } else {
        std::cout << "Pointer is null." << std::endl;
    }
}

int main() {
    int* ptr1 = new int(42);
    printValue(ptr1);

    int* ptr2 = nullptr;
    printValue(ptr2);

    delete ptr1;

    return 0;
}