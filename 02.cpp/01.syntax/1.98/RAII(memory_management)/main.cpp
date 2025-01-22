#include <iostream>

//Q. what is RAII?
//A. Resource Acquisition Is Initialization
//   RAII is a programming idiom that ensures that 
//   "resources are released automatically when they are no longer needed."
//   생상자에서 private field에 메모리 할당해주고, 소멸자에서 메모리 해제해준다. 수동으로 해제하는거 까먹을 수 있으니까.
class Resource {
private:
    int* data;
public:
    Resource() : data(new int[10]) {
        std::cout << "Resource created" << std::endl;
    }

    ~Resource() {
        std::cout << "Resource destroyed" << std::endl;
        delete[] data;
    }

    void useResource() {

    }
};

int main() {
    Resource res;
    res.useResource();

    return 0;
}