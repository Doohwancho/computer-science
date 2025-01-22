#include <iostream>
#include <stdexcept>

/**
 * @brief 
 * linkedlist로 데이터 보관, priority 기준으로 enqueue()시 사이에 낑겨넣음
 * enqueue's time complexity = O(N)
 * dequeue's time complexity = O(1)
 */
template<typename T> //namespace -> template
class PriorityQueue {
private:
    //Q. dont i need pointer to class Node? what kind of datastructure do i need?
    // Node queue[SIZE]; //error - unknown type error 'Node', Node 선언 이후에 해줘야 하는 듯 하다.

    //Q. 어떤 datastructure에 pq 데이터 관리해야 함?
    struct Node { //Q. class vs struct?
    public:
        T data;
        int priority;
        Node* next;
    };
    Node* front; //arraylist인 Node front[]; 보다 linkedlist로 만들어야 priority가 애매해서 중간에 삽입할 때 유리
    int size;

public:
    PriorityQueue() : front(nullptr), size(0) {}
    void enqueue(const T& value, int priority) {
        //Q. create NodeClass with value for data, with priority? or not?
        Node* newNode = new Node; //Node newNode; 하면, newNode.data가 되는데, Node* newNode; 하면, newNode->data 라고 함
        newNode->data = value;
        newNode->priority = priority;
        newNode->next = nullptr;

        //Q. how to insert to datastructure while considering priority?
        //A. 첫 노드부터 돌면서 비교, priority가 높으면 pass, 낮으면, 해당 노드의 before 노드.next, 새로만든 노드.next = 현 노드로 이어줌
        if(isEmpty() || priority > front->priority) {
            newNode->next = front;
            front = newNode;
        } else {
            Node* curr = front;
            while(curr->next && curr->next->priority >= priority) {
                curr = curr->next;
            }
            newNode->next = curr->next;
            curr->next = newNode;
        }
        size++;
    }
    T dequeue() {
        if(isEmpty()) {
            throw std::runtime_error("priority queue is empty");
        }
        Node temp = front;
        T firstValue = temp.data;
        front = front->next;
        delete temp;
        size--;
        return firstValue;
    };
    bool isEmpty() {
        return size == 0;
    };
    void print() {
        Node* curr = front;
        while(curr) {
            std::cout << "(" << curr->data << "," << curr->priority << ")";
            curr = curr->next;
        }
        std::cout << std::endl;
    };
};

int main() {
    PriorityQueue<float> pq;

    pq.enqueue(10.1, 1);
    pq.enqueue(3.3, 10);
    pq.enqueue(5.8, 7);

    pq.print();
}