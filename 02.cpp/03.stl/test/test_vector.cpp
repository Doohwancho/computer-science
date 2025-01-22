#include <iostream>
#include "vector.hpp"

void test_push_back() {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    if (v.get_size() != 3) {  
        std::cerr << v.get_size();
        std::cerr << "test_push_back failed: size is incorrect\n";
        exit(1);
    }
    if (v[0] != 1 || v[1] != 2 || v[2] != 3) {
        std::cerr << "test_push_back failed: elements are incorrect\n";
        exit(1);
    }
    std::cout << "test_push_back passed\n";
}

void test_resize() {
    Vector<int> v;
    v.resize(5);

    int curr_size = v.get_size();
    std::cout << "current size of element: " << curr_size << std::endl;

    int curr_capacity = v.get_capacity();
    std::cout << "current capacity of vector: " << curr_capacity << std::endl;

    if (v.get_capacity() != 5) {
        std::cerr << "test_resize failed: capacity is incorrect\n"; //주의! size는 실제 담긴 원소의 갯수, capacity는 vector가 담을 수 있는 원소의 총 갯수
        exit(1);
    }
    std::cout << "test_resize passed\n";
}

int main() {
    test_push_back();
    test_resize();
    std::cout << "All tests passed.\n";
    return 0;
}