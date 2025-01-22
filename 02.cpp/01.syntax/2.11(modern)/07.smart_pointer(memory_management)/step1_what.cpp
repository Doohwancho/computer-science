#include <iostream>

//왜 씀?
//실 개발에서는 new 쓰는거 안 좋게 여김 (C++ Core Guidelines R.11). unique/shared pointer를 써야지.
//new 는 delete해주지않으면 ㅈ될수 있어서

int main() {
    //case1) unique pointer
    std::unique_ptr<int> ptr = std::make_unique<int>(42);

    //what is unique pointer?
    //      unique pointer is a pointer that can only be owned by one object.
    //      When the object that owns the unique pointer is destroyed, the pointer is also destroyed.

    //case2) what is shared pointer?
    //       shared pointer is a pointer that can be shared by multiple objects.
    //       When the last shared pointer to an object is destroyed, the object is destroyed.
    std::shared_ptr<std::string> sharedPtr = std::make_shared<std::string>("hello");

    //case3) weak pointer example
    std::weak_ptr<std::string> weakPtr = sharedPtr;

    //what is weak pointer?
    //weak pointer is a pointer that does not prevent its referent from being destroyed.
    //It does this by holding a weak reference to the referent.

    return 0;
}