#include <iostream>
#include <vector>

template<typename T>
class myStack {
    private:
        std::vector<T> items;
    public:
        bool isEmpty() {
            return items.empty();
        }
        void push(const T& value) {
            items.push_back(value);
        }

        T pop() {
            if(isEmpty()) {
                throw std::exception();
                return T(); //default value for type T
            }
            T top = items.back();
            items.pop_back();
            return top;
        }

        T peek() {
            if(isEmpty()) {
                throw std::exception();
                return T(); //default value for type T
            }
            return items.back();
        }

        int size() {
            return items.size();
        }

};

int main() {
    myStack<int> myIntStack;
    myStack<float> myFloatStack;

    myIntStack.push(1);
    myIntStack.push(2);
    myIntStack.push(3);
    myIntStack.push(4);
    myIntStack.push(5);

   myFloatStack.push(1.1);
   myFloatStack.push(2.2);
   myFloatStack.push(3.3);
   myFloatStack.push(4.4);
   myFloatStack.push(5.5);
   
    std::cout << "myIntStack.peek(): " << myIntStack.peek() << std::endl; //5
    std::cout << "myFloatStack.peek(): " << myFloatStack.peek() << std::endl; //5.5
}