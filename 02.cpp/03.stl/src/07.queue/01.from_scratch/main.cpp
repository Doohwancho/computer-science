#include <iostream>

#define SIZE 5

template<typename T>
class Queue {
private:
    T* queue;
    int front, rear;

public:
    Queue(int size = SIZE) : front(-1), rear(-1) {
        queue = new T[size];
    }
    ~Queue() {
        delete[] queue; //deallocate memory allocated for queue
    }

    bool isEmpty() {
        return front == -1;
    }
    bool isFull() {
        return rear == SIZE - 1;
    }
    void enqueue(const T& value) { //const를 붙여야 both lvalue & rvalue를 넣을 수 있다. 안붙이면 lvalue만 넣을 수 있다.
        if(isFull()) {
            std::cout << "Queue is full" << std::endl;
            return;
        }
        if(isEmpty()) {
            std::cout << "Queue is empty" << std::endl;
            front = rear = 0;
        } else {
            rear = (rear + 1) % SIZE;
        }
        queue[rear] = value;
    }
    T dequeue() {
        if(isEmpty()) {
            std::cout << "Queue is empty" << std::endl;
            return T();
        }
        //방법론1 - 앞에거 하나 빼고, 전부 한칸씩 앞으로 밀기. 비효율
        // T value = queue[front];
        // for(int i = 0; i < SIZE - 1; i++) {
        //     queue[i] = queue[i + 1];
        // }
        // queue[rear] = T();
        // rear -= 1;
        // return value;

        //방법론2 - 앞에거 하나 빼고, front 포인터 한칸 오른쪽으로 밀기. circular하게 동작함.
        T value = queue[front];

        if(front == rear) {
            front = rear = -1;
        } else {
            front = (front + 1) % SIZE;
        }
        return value;
    }
    T peek() {
        if(isEmpty()) {
            std::cout << "Queue is empty" << std::endl;
            return T();
        }
        return queue[front];
    }
};

int main() {
    Queue<int> myIntQueue;
    Queue<float> myFloatQueue;

    int a = 1;
    int b = 2;
    int c = 3;

    //case1) enqueue(T& value) //cannot take rvalue. can only take lvalue
    myIntQueue.enqueue(a);
    myIntQueue.enqueue(b);
    myIntQueue.enqueue(c);

    //case2) enqueue(const T& value) //can take rvalue with const
    myIntQueue.enqueue(100);
    myIntQueue.enqueue(200);
    myIntQueue.enqueue(300); //size가 5라 이건 안들어감

    std::cout << "Front element: " << myIntQueue.peek() << std::endl;

    std::cout << "Dequeueing elements: ";
    while (!myIntQueue.isEmpty()) {
        std::cout << myIntQueue.dequeue() << " ";
    }
    std::cout << std::endl;
    std::cout << "queue is not empty: " << myIntQueue.isEmpty() << std::endl;

    return 0;
}