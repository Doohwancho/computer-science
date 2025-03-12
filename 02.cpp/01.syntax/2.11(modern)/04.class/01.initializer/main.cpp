#include <iostream>
#include <vector>
#include <initializer_list>

class MyClass {
public:
    MyClass(std::initializer_list<int> list) {
        for (int value : list) {
            data.push_back(value);
        }
    }

    void printData() {
        for (int value : data) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<int> data;
};

int main() {
    MyClass obj1 = {1, 2, 3, 4, 5};
    obj1.printData();

    MyClass obj2{10, 20, 30};
    obj2.printData();

    return 0;
}