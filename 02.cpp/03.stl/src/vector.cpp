#ifndef VECTOR_H
#define VECTOR_H
#include <cstddef>
#include <iostream>
#include <stdexcept> //for exception class

template<typename T>
class Vector {
public:
    Vector(): data(nullptr), capacity(0), size(0) {}  // Changed count to size for consistency
    ~Vector() { delete[] data; }
    
    std::size_t get_size() const { return size; }  
    
    //vector의 capacity size를 조절하는 함수.
    //오해) capacity는 원소를 담을 수 있는 총 량이고, size는 실제 담겨있는 원소의 총 량이다. 
    void resize(std::size_t new_capacity) {
        if (new_capacity <= capacity) return;
        
        std::cout << "Resizing from " << capacity << " to " << new_capacity << std::endl;

        T* new_data = new T[new_capacity];
        for (std::size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }

        // free old memory 
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }
    
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