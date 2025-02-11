#include <stdio.h>
#include <stdlib.h> //malloc()


typedef struct Node {
    int data;
    struct Node* next; //주의! struct 안쓰면 에러남!
} Node;


Node* createEmptyNode() { //주의! c에는 function overloading 기능 지원 안한다!
    Node* node = (Node*)malloc(sizeof(Node));
    if(node == NULL) {
        printf("memory allocation failed");
        exit(1);
    }

    node->data = 0;
    node->next = NULL;

    return node;
}

Node* createNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if(node == NULL) {
        printf("memory allocation failed");
        exit(1);
    }

    node->data = data;
    node->next = NULL;

    return node;
}

void insert(Node** head, int data) {
    Node* node = createNode(data);

    //head가 NULL이면 삽입 후 반환
    if(*head == NULL) {
        *head = node;
        return;
    }

    //add newly created node to at the end of head 
    Node* curr = *head;
    while(curr->next != NULL) { //Q. 이렇게 하면 왜 segfault? A. curr이 NULL될 떄까지 curr->next; 하기 때문에 
        curr = curr->next;
    }
    curr->next = node; //여기서 segfault걸림.
}

void printLinkedList(Node* head) {
    while(head != NULL) {
        printf("data: %d\n", head->data);
        head = head->next;
    }
}

void freeLinkedList(Node* head) {
    //내꺼 
    // if(head == NULL) {
    //     return;
    // }
    // Node* prev = head;
    // head = head->next;

    // if(head == NULL) {
    //     free(prev);
    // }

    // while(head != NULL) {
    //     free(prev);
    //     prev = head;
    //     head = head->next;
    // }
    
    //solution
    Node* prev;
    while(head != NULL) {
        prev = head; //Q. 복사되나? A. ㅇㅇ
        head = head->next;
        free(prev);
    }
}

int main() {
    Node* head = NULL;
    int data1 = 10;
    int data2 = 20;
    int data3 = 30;
    
    head = createEmptyNode();

    insert(&head, data1); //call by value로 넘기는데 c에서는 default니까, head 넘기면 원본이 파라미터에 복사되서 원본이 수정안됨. 그래서 원본 주소의 복사본을 call by value로 넘겨야 한다.
    insert(&head, data2);
    insert(&head, data3);

    printLinkedList(head);  //원본 데이터 수정, 삽입, 삭제 없으니까 그냥 call by value로 넘긴다.

    freeLinkedList(head); //free()가 주솟값 안넘겨도 되니까 그냥 원본 넘긴다.

    return 0;
}
