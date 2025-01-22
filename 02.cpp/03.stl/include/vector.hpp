#ifndef VECTOR_H
#define VECTOR_H
#include <cstddef>
#include <iostream>
#include <stdexcept>

template<typename T>
class KirbyVector {
public:
    // 1. Constructors 
    KirbyVector(): data(nullptr), capacity(0), size(0) {}
    ~KirbyVector() { delete[] data; }
    KirbyVector(std::size_t initial_capacity) 
        : data(new T[initial_capacity]), capacity(initial_capacity), size(0) {}
    // Copy constructor
    KirbyVector(const KirbyVector& other) 
        : data(new T[other.capacity]), capacity(other.capacity), size(other.size) {
        for (std::size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // 2. Assignment operators
    //1. T& is reference to type T, ex. vec[0] = 42, both read and write
    //without &, you would get a copy, instead of a reference to the actual element.
    //2. operator[] = means operator to []
    //3. std::size_t is guaranteed to be large enough to represent any size in memory. int could sometimes be small. std::size_t is standard
    T& operator[](std::size_t index) {
        if (index >= size) {  
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    KirbyVector& operator=(const KirbyVector& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            size = other.size;
            data = new T[capacity];
            for (std::size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    // 3. Capacity 
    bool empty() const { 
        return size == 0;
    }
    std::size_t get_size() const { return size; }
    std::size_t get_capacity() const { return capacity; }

    // 4. Modifiers
    //Q. why implement on header file?
    //A. cpp template(generics)에서는 컴파일러가 컴파일 타임 때 특정 타입 T에 대해 full implementation code가 필요하다고 함. 그래서 헤더 파일에 모든걸 구현한다고 함 
    void push_back(const T& value) {
        // std::cout << "----------- push_back -----------\n";
        // std::cout << "Pushing back value: " << value << std::endl;
        // std::cout << "Before push_back - size: " << size << ", capacity: " << capacity << std::endl;

        if (size == capacity) {  
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = value;  

        // std::cout << "After push_back - size: " << size << ", capacity: " << capacity << std::endl;
        // print();
        // std::cout << "------------------------------\n";
    }

    //remove all elements
    void clear() {
        size = 0; // Note: 메모리 해제하지 않았음.  Q. 모든 원소 for-loop 돌면서 메모리 해제 해야하지 않을까?
    }

    //insert at that position
    void insert(std::size_t pos, const T& value) {
        if(pos > size) {
            throw std::out_of_range("insert position out of range");
        }

        if(size == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }

        for(std::size_t i = size; i > pos; --i) {
            data[i] = data[i-1];
        }

        data[pos] = value;
        ++size;
    }

    //remove at that position
    void erase(std::size_t pos) {
        if(pos >= size) {
            throw std::out_of_range("erase position out of range");
        }

        for(std::size_t i = pos; i < size - 1; ++i){
            data[i] = data[i+1];
        }

        --size;
    }
    

    // 5. Iterator support 
    using iterator = T*;
    using const_iterator = const T*;
    iterator begin() { return data; }
    iterator end() { return data + size; }
    const_iterator begin() const { return data; }
    const_iterator end() const { return data + size; }

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

    //.cpp에 구현하면 에러난다. 헤더파일에 남겨놔야 한다. 
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