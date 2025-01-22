#ifndef VECTOR_H
#define VECTOR_H
#include <cstddef>
#include <iostream>
#include <stdexcept> //for exception class

template<typename T>
class KirbyVector {

// Q. 헤더파일에 public 다 구현해 놓으니까, .cpp에 구현 안해도 컴파일 되고 돌아가네??
// public:
//     KirbyVector(): data(nullptr), capacity(0), size(0) {}  
//     ~KirbyVector() { delete[] data; }

private:
    T* data;
    std::size_t capacity; //vector가 원소를 수용할 수 있는 최대 사이즈 
    std::size_t size; //vector에 원소가 실제로 들어있는 양 
};

#endif // VECTOR_H
