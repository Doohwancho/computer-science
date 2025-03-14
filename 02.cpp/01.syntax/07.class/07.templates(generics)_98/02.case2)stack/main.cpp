#include <iostream>
#include <vector>
#include <string>

// 함수 템플릿
template <typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// 클래스 템플릿
template <typename T>
class Stack {
private:
    std::vector<T> elements;

public: 
    void push(const T& item) {
        elements.push_back(item);
    }

    T pop() {
        if(elements.empty()) {
            throw std::out_of_range("스택 비었다");
        }

        T top = elements.back();
        elements.pop_back();
        return top;
    }

    bool isEmpty() const {
        return elements.empty();
    }

    size_t size() const {
        return elements.size();
    }
};


int main() {
    // 함수 템플릿 사용
    std::cout << "정수 최댓값" << max(10, 20) << std::endl;
    std::cout << "실수 최댓값" << max(10.5, 20.5) << std::endl;
    // std::cout << "문자열 최댓값" << max(std::string("abc"), std::string("def")) << std::endl;

    // 클래스 템플릿 사용
    Stack<int> intStack;
    intStack.push(1);
    intStack.push(2);
    intStack.push(3);

    std::cout << "스택 크기: " << intStack.size() << std::endl;
    std::cout << "pop: " << intStack.pop() << std::endl;
    std::cout << "pop: " << intStack.pop() << std::endl;

    Stack<std::string> stringStack;
    stringStack.push("hello");
    stringStack.push("world");

    std::cout << "팝한 값: " << stringStack.pop() << std::endl;

    return 0;
}
