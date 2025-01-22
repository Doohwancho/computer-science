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

//case1) Node**를 받아 -> insert a new node at the end of the linked list
void insert(Node** head, int data) { 
    //Q. 왜 Node**임? Node*로 안받고? 
    //A. call by value -> Node*로 받아서 createNode(data);로 받아 넣으면 원본 head가 변하지 않음.
    //   따라서 원본 Node* head; 에 주솟값을 받아서 값을 변경해줘야 함.
    Node* newNode = createNode(data); //newNode를 만들면, Node* 포인터 값을 받음. 

    if(*head == NULL) {
        *head = newNode;
    } else {
        Node* temp = *head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

//case2) 그냥 Node*만 받아 -> insert a new node at the end of the linked list
// void insert(Node* head, int data) { //passed by value. receives copy of head pointer(Node*). any change to head will not affect the original head.
//     printf("pass by value, head inside function: %d \n", head); //1465935920 - same address?
//     printf("pass by value, head inside function's address: %d \n", &head); //1869933224 - different address to head pointer
//     Node* newNode = createNode(data);

//     if(head == NULL) {
//         head = newNode;
//     } else {
//         Node* temp = head;
//         while(temp->next != NULL) {
//             temp = temp->next;
//         }
//         temp->next = newNode;
//     }
// }

void printLinkedList(Node* head) {
    Node* node = head;

    //case1) head와 node 둘 다 같은 주소를 가르킨다 -> free(node) 하면 free(head)와 같다.
    printf("%d \n", head); 
    printf("%d \n", node); 

    //case2) head와 node는 다른 변수기 때문에 다른 메모리 주소를 가지고 있다.
    printf("%d \n", &head); //1797352104
    printf("%d \n", &node); //1797352096

    //case3) head, node 둘다 포인터가 가르키는 값, *을 찍으면 %d 값이 나온다.
    printf("%d \n", *head); //10 - head가 가르키는 첫번째 노드의 int 값인 10이 나온다.
    printf("%d \n", *node); //10 - node가 가르키는 첫번째 노드의 int 값인 10이 나온다. 

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
    int data1 = 10;
    int data2 = 20;
    int data3 = 30;
    Node* head = NULL;

    //experiment) insert(Node*) vs insert(Node**), call by value vs call by reference 테스트
    // case1) call by reference, Node**를 받는 케이스. why? passed by reference
    insert(&head, data1); //head 포인터의 주솟값을 넘긴다.
    insert(&head, data2); //head 포인터의 주솟값을 넘긴다.
    insert(&head, data3); //head 포인터의 주솟값을 넘긴다.

    // case2) call by value, Node*를 받는 케이스 -> wrong, why? passed by value라서. 
    // printf("hello\n");
    // head = createNode(data2); //head 포인터의 주솟값을 넘긴다.
    // printf("pass by value, head outside function: %d \n", head); //1465935920 - head 포인터가 가르키는 값의 주소값은 insert안에서 찍은 head값과 같다.
    // printf("pass by value, head outside function's address: %d \n", &head); //1869933272 - 그런데 head 포인터의 주솟값은 insert() 밖과 안이 다르다. call by value니까, head를 인수로 넘기면 새로운 파라미터 variable을 만들어서 받는다. 물론 포인터의 value인 주솟값은 같다.
    // insert(head, data1); //head 포인터 그 자체를 넘긴다.

    printLinkedList(head);

    freeLinkedList(head);

    return 0;
}