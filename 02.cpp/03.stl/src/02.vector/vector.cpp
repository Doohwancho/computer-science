#ifndef VECTOR_H
#define VECTOR_H
#include <cstddef>
#include <iostream>
#include <stdexcept> //for exception class

template<typename T>
class KirbyVector {
public:
    KirbyVector(): data(nullptr), capacity(0), size(0) {}  
    ~KirbyVector() { delete[] data; }
    
    std::size_t get_size() const { return size; }  
    
    
    void push_back(const T& value) {
        if (size == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = value;
    }
    
    T& operator[](std::size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");  
        }
        return data[index];
    }

private:
    T* data;
    std::size_t capacity; //vector가 원소를 수용할 수 있는 최대 사이즈 
    std::size_t size; //vector에 원소가 실제로 들어있는 양 
};

#endif // VECTOR_H
