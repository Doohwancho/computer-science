#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/********************************
 * Private Functions 
 */ 
static Node* create_node(void* data, size_t size) { //여기서 size는 자료형의 사이즈. ex) int형의 사이즈 
    Node* node = (Node*)malloc(sizeof(Node)); //자료구조 'Node'만큼 메모리 확보
    if(!node) return NULL; //Node_size 만큼 메모리 없다? 반환

    // Allocate memory for data and copy it 
    void* data_copy = malloc(size); //data의 자료형 만큼의 메모리 확보. c엔 generic 없어서 void* 타입 쓰는 것. size로 자료형의 사이즈를 명시에서 그걸로 타입 유추
    if(!data_copy) {  //메모리 확보가 안됬으면, 
        free(node); //Node의 메모리 초기화 
        return NULL; //break
    }

    memcpy(data_copy, data, size); //확보한 메모리에 data를 덮어 씌운다.
    node->data = data_copy; //Node에 데이터에 데이터 삽입
    node->size = size; //해당 데이터의 자료형만큼 사이즈를 명시한다. 
    node->next = NULL; //다음 노드는 NULL

    return node;
}




/********************************
 * Core Linkedlist operations
 */
LinkedList* list_create(void) {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList)); //malloc으로 LinkedList 자료형의 크기만큼 메모리할당 받으면, void*(generic)으로 반환하는데, 이걸 LinkedList* 타입으로 명시한다.
    if(!list) return NULL;
    
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;

    return list;
}

static void free_node(Node* node) {
    if(node) {
        free(node->data); //노드 내부에 참조하는 node->data도 free() 해줘야 한다!
        free(node);
    }
}

void list_clear(LinkedList* list) {
    if(!list) return;
    
    Node* current = list->head;
    while(current) {
        Node* next = current->next;
        free_node(current);
        current = next;
    }

    list->head = list->tail = NULL;
    list->length = 0;
}

void list_destroy(LinkedList* list) {
    if(!list) return;

    list_clear(list);
    free(list);
}

bool list_is_empty(const LinkedList* list) {
    return list ? list->length == 0 : true;
}

size_t list_length(const LinkedList* list) {
    return list ? list->length : 0;
}


/********************************
 * Node Operations 
 */
bool list_append(LinkedList* list, void* data, size_t size) { //여기서 size는 자료형의 사이즈. ex) int형의 사이즈 
    if(!list || !data) return false;

    Node* node = create_node(data, size);
    if(!node) return false;

    if(list->tail) {
        list->tail->next = node; //append()
        list->tail = node; //마지막 tail을 노드로 옮기기
    } else { //처음 insert라면, head && tail은 같아진다. 
        list->head = list->tail = node;
    }

    list->length++;
    return true;
}

bool list_remove_at(LinkedList* list, size_t index, void** out_data) { 
    if(!list || index >= list->length) return false;

    Node* to_remove;
    if(index == 0) {
        to_remove = list->head;
        list->head = list->head->next;
        if(!list->head) list->tail = NULL;
    } else {
        Node* current = list->head;
        for(size_t i = 0; i < index -1; i++) {
            current = current->next;
        }

        to_remove = current->next;
        current->next = to_remove->next;
        
        if(to_remove == list->tail) {
            list->tail = current;
        }
    }
    //Q. 왜 void**? 
    //A. 함수 자체적으로는 삭제 여부를 bool로 반환하지만, 
    //함수 외부에서는 제거된 out_data를 확인할 때, 
    //```
    //void* removed_data; 
    //list_remove_at(list, 0, &removed_data);
    //int* removed_int = (int*)removed_data; 
    //```
    //이런식으로 확인한다고 함. 범용성 이슈로 void** 쓴다고 함.
    if (out_data) {
        *out_data = to_remove->data;
    } else {
        free(to_remove->data);
    }

    free(to_remove);
    list->length--;
    return true;
}

bool list_insert_at(LinkedList* list, size_t index, void* data, size_t size) {
    if(!list || !data) return false;
    if(index > list->length) return false;

    if(index == 0) {
        Node* node = create_node(data, size);
        if(!node) return false;
        
        node->next = list->head;
        list->head = node;
        if(!list->tail) {  // 리스트가 비어있었다면
            list->tail = node;
        }
        list->length++;
        return true;
    }
    if(index == list-> length) return list_append(list, data, size);
    
    Node* node = create_node(data, size);
    if(!node) return false;

    Node* current = list->head;
    for(size_t i = 0; i < index -1; i++) {
        current = current->next;
    }

    node->next = current->next; 
    current->next = node;
    list->length++;

    return true;
}
void* list_get_at(const LinkedList* list, size_t index) {
    if(!list || index >= list->length) return NULL;
    
    Node* current = list->head;
    for(size_t i = 0; i < index; i++) {
        current = current->next;
    }

    return current->data;
}


bool list_set_at(LinkedList* list, size_t index, void* data, size_t size) {
    if(!list || index >= list->length) return false;

    //step1) find a place in the list for set
    Node* current = list->head;
    for(size_t i = 0; i < index; i++) {
        current = current->next;
    }

    void* new_data = malloc(size);
    if(!new_data) return false;
    
    memcpy(new_data, data, size);
    free(current->data);
    current->data = new_data;
    current->size = size;

    return true;
}

// Iterator functions
typedef void (*ListIteratorFn)(void* data, size_t size, void* user_data);
void list_foreach(const LinkedList* list, ListIteratorFn fn, void* user_data) {
    if(!list || !fn) return;

    for(Node* current = list->head; current; current = current->next) {
        fn(current->data, current->size, user_data);
    }
}

// Search Operations
// Q. what is ssize_t? diff size_t and ssize_t?
// A. size_t = 0~INT_MAX (non-negative value), use-case: 메모리에서 size of object 명시할 때 쓰임. ex. sizeof(int) * 10
//    ssize_t = -1 ~ INT_MAX (negative number can exist), use-case: Commonly used in system calls like read() or write(), where a negative return value signals an error.
ssize_t list_find(const LinkedList* list, const void* data, size_t size) {
    if(!list || !data) return -1;

    size_t index = 0;
    for(Node* current = list->head; current; current = current->next) {
        if(current->size == size && memcmp(current->data, data, size) == 0) { //Q. 왜 find()에 index나 data말고 size가 필요함? -> 데이터 비교연산할 때 size가 필요함.
            return index;
        }
        index++;
    }
    return -1;
}
