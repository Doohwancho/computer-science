#ifndef VECTOR_H
#define VECTOR_H
#include <cstddef>
#include <iostream>
#include <stdexcept>

template<typename T>
class KirbyVector {
public:
    KirbyVector(): data(nullptr), capacity(0), size(0) {}
    ~KirbyVector() { delete[] data; }
    
    std::size_t get_size() const { return size; }
    std::size_t get_capacity() const { return capacity; }
    
    //Q. why implement on header file?
    //A. cpp template(generics)에서는 컴파일러가 컴파일 타임 때 특정 타입 T에 대해 full implementation code가 필요하다고 함. 그래서 헤더 파일에 모든걸 구현한다고 함 
    void push_back(const T& value) {
        std::cout << "----------- push_back -----------\n";
        std::cout << "Pushing back value: " << value << std::endl;
        std::cout << "Before push_back - size: " << size << ", capacity: " << capacity << std::endl;

        if (size == capacity) {  
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = value;  

        std::cout << "After push_back - size: " << size << ", capacity: " << capacity << std::endl;
        print();
        std::cout << "------------------------------\n";
    }
    
    T& operator[](std::size_t index) {
        if (index >= size) {  
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void print() const {
        std::cout << "------------ print ------------\n";
        std::cout << "Vector contents (size =" << size << ", capacity=" << capacity << "): ";
        for (std::size_t i = 0; i < size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    T* data;
    std::size_t capacity; //vector가 원소를 담을 수 있는 총량 
    std::size_t size;  //실제 vector에 담겨있는 원소들의 수량 

    //vector의 capacity size를 조절하는 함수.
    //오해) capacity는 원소를 담을 수 있는 총 량이고, size는 실제 담겨있는 원소의 총 량이다. 
    void resize(std::size_t new_capacity) {
        if (new_capacity <= capacity) return;
        
        // std::cout << "Resizing from " << capacity << " to " << new_capacity << std::endl;

        T* new_data = new T[new_capacity];
        for (std::size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }

        // free old memory 
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

    template<typename U> //template을 쓸꺼면 test_resize<int>(); 이런 식으로 불러야 함.
    friend void test_resize(); //private function에 대해 test code쓸 땐 friend 키워드 쓰면 좋다.
};

#endif // VECTOR_H