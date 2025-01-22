#include "vector.hpp"
#include <iostream>
#include <cassert>
#include <stdexcept>

void test_index_out_of_range() {
    KirbyVector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    //1) test valid access 
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);

    //2) test modify element
    v[1] = 100;
    assert(v[1] == 100);

    //3) test 'out of range' access
    bool exception_thrown = false;
    try {
        v[4]; //this should throw exception
    } catch (const std::out_of_range& e) {
        exception_thrown = true;
    }

    std::cout << exception_thrown << std::endl;

    //4) insert type that is not the same type of declared one 
    char c = 'A';
    v.push_back(c);
    assert(v[3] == 65); //<int>에 char 넣으면, ascii of that char로 들어감. 'A' == 65
    std::cout << v[3] << std::endl;

    //5) insert float time to int vector will truncate its value 
    float pi = 3.14f;
    v.push_back(pi);
    assert(v[4] == 3); //3.14 is truncated to 3
    std::cout << v[4] << std::endl;

    std::cout << "test_index_out_of_range() passed\n";
}

void test_push_back() {
    KirbyVector<int> v;
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

template<typename T>
void test_resize() {
    KirbyVector<int> v;
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
    test_resize<int>();
    test_index_out_of_range();
    std::cout << "All tests passed.\n";
    return 0;
}