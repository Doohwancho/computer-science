#include <stdio.h>
#include <stdlib.h>

/*
    takeaways

    1. typedef struct
    2. Node*에 변수에 접근할 땐 ->을 쓰고, 포인터가 아닌 instance에서 값을 참조할 땐 .을 써서 접근함.
    3. c에서 insert(Node* node); 인수로 넘기면, insert(Node* node);로 받을 때, call by value로 받기 때문에, node 파라미터에 값을 수정해도, 원본 node는 변화 없다.
    4. 따라서 insert(Node** node); 로 포인터의 주솟값을 넘기면, call by reference로 받기 때문에, node 파라미터에 값을 수정하면, 원본 node가 변화한다.
*/

typedef struct Node {    
    int data;
    struct Node* next; //warn! - 그냥 Node* next;하면 에러남. 
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL) {
        printf("Memory Allocation Failed.\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;

    return newNode;
}

//case1) 그냥 Node*로 받는 경우
void insertCallByValue(Node* head, int data) { //Node* head == call by value
    // 원본 head의 주솟값 printf(&original_head); 랑, 
    // 파라미터로 받은 head의 주솟값 printf(&head); 해보면, 다르다!
    // 왜냐면 c는 기본적으로 call by value이기 때문.
    Node* newNode = createNode(data);

    if(head == NULL) {
        head = newNode; // 이 변경은 원본 head에 영향을 주지 않음
    } else {
        Node* temp = head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

//case2) Node**로 받는 경우
//Q. 왜 Node**임? Node*로 안받고? 
//A. c는 파라미터로 변수 넘길 때 기본적으로 call by value임. 
//   근데 Node* head로 받아서 head = newNode; 하면, call by value로 복사된 head에 넣는거라 원본데이터에 insert가 안됨.
//   근데 Node** head로 파라미터로 받으면, call by value로 "주솟값"을 복사해옴.
//   그 주솟값에 *(주솟값) = newNode; 해서 원본 데이터에 insert하는 것.
void insertCallByReference(Node** head, int data) { 
    if (head == NULL) {
        printf("Invalid head pointer reference.\n");
        return;
    }
    Node* newNode = createNode(data); 

    if(*head == NULL) {
        *head = newNode; // 이 변경은 원본 head를 변경함
        return;
    }
    
    Node* temp = *head;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}


void printLinkedList(Node* head) {
    Node* node = head;

    while(node != NULL) {
        printf("value: %d\n", node->data);
        node = node->next;
    }
}


void freeLinkedList(Node* node) {
    Node* prev;
    while(node != NULL) {
        prev = node; 
        node = node->next;
        free(prev);
    }
}

int main() {
    Node* headCallByValue = NULL;
    int data1 = 10;
    int data2 = 20;
    int data3 = 30;

    Node* headCallByReference = NULL;
    int data4 = 40;
    int data5 = 50;
    int data6 = 60;

    //experiment) insert(Node*) vs insert(Node**), call by value vs call by reference 테스트
    // case1) call by value, Node*를 받는 케이스 -> wrong, why? passed by value라서. 

    insertCallByValue(headCallByValue, data1); //head 포인터 그 자체를 넘긴다.
    insertCallByValue(headCallByValue, data2); //head 포인터 그 자체를 넘긴다.
    insertCallByValue(headCallByValue, data3); //head 포인터 그 자체를 넘긴다.

    // case2) call by reference, Node**를 받는 케이스. why? passed by reference
    insertCallByReference(&headCallByReference, data4); //head 포인터의 주솟값을 넘긴다.
    insertCallByReference(&headCallByReference, data5); //head 포인터의 주솟값을 넘긴다.
    insertCallByReference(&headCallByReference, data6); //head 포인터의 주솟값을 넘긴다.

    
    //print
    printf("After insertWithPointer: head1 = %p\n", (void*)headCallByValue);  // head1은 여전히 NULL
    printLinkedList(headCallByValue); // does not print anything!!!!!!!!!! why? call by value -> 원본데이터에 안 insert()됨.

    printf("After insertWithDoublePointer: head2 = %p\n", (void*)headCallByReference);  // head2가 새 노드를 가리킴
    printLinkedList(headCallByReference);

    //free
    freeLinkedList(headCallByValue);
    freeLinkedList(headCallByReference);

    return 0;
}
