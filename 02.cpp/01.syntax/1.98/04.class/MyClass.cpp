#include "MyClass.h"

//Constructor
MyClass::MyClass(int value) : myPrivateMember(value) {}

void MyClass::setPrivateMember(int value) {
    myPrivateMember = value;
}

int MyClass::getPrivateMember() {
    return myPrivateMember;
}