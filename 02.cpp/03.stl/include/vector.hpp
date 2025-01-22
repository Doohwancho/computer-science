#ifndef VECTOR_H
#define VECTOR_H
#include <cstddef>
#include <iostream>
#include <stdexcept>

template<typename T>
class Vector {
public:
    Vector(): data(nullptr), capacity(0), count(0) {}
    ~Vector() { delete[] data; }
    
    std::size_t get_size() const { return count; }
    std::size_t get_capacity() const { return capacity; }
    
    void resize(std::size_t new_size) {
        std::cout << "----------- resize -----------\n";
        if (new_size <= capacity) {
            // count = new_size; 
            return;
        }

        std::cout << "Resizing from " << capacity << " to " << new_size << std::endl;

        // Allocate new memory
        T* new_data = new T[new_size];
        for (std::size_t i = 0; i < count; ++i) {
            new_data[i] = data[i];
        }

        // Free old memory
        delete[] data;
        data = new_data;
        capacity = new_size;
        // count = new_size;

        std::cout << "-------------------------\n";
    }
    
    void push_back(const T& value) {
        std::cout << "----------- push_back -----------\n";
        std::cout << "Pushing back value: " << value << std::endl;
        std::cout << "Before push_back - count: " << count << ", capacity: " << capacity << std::endl;

        if (count == capacity) {  // Changed size to count
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[count++] = value;  // Changed size to count

        std::cout << "After push_back - count: " << count << ", capacity: " << capacity << std::endl;
        print();
        std::cout << "------------------------------\n";
    }
    
    T& operator[](std::size_t index) {
        if (index >= count) {  // Changed size to count
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void print() const {
        std::cout << "------------ print ------------\n";
        std::cout << "Vector contents (count=" << count << ", capacity=" << capacity << "): ";
        for (std::size_t i = 0; i < count; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    T* data;
    std::size_t capacity;
    std::size_t count;  // Consistently using count instead of size
};

#endif // VECTOR_H