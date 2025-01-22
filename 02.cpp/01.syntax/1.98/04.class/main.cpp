#include <iostream>
// #include "MyClass.h"

class MyClass {
    private:
        int myPrivateMember;

    public:
        MyClass(int value): myPrivateMember(value) {}
        
        void setPrivateMember(int value) {
            myPrivateMember = value;
        }
        int getPrivateMember() {
            return myPrivateMember;
        };
};

int main() {
    MyClass obj(42);
    std::cout << obj.getPrivateMember() <<std::endl;

    obj.setPrivateMember(100);
    std::cout << obj.getPrivateMember() <<std::endl;
}