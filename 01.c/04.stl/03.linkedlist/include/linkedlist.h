#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>  // for ssize_t

// forward declaration of the node structure
typedef struct Node Node;

struct Node {
    void *data; //왜 void type임? c에는 generics(templates) 없음?
    size_t size;  //여기서 size는 자료형의 사이즈. ex) int형의 사이즈 
    Node* next;
};

typedef struct {
    Node* head;
    Node* tail;
    size_t length;
} LinkedList;


// Core Linkedlist operations
LinkedList* list_create(void);
void list_destroy(LinkedList* list);
bool list_is_empty(const LinkedList* list);
size_t list_length(const LinkedList* list);
void list_clear(LinkedList* list);


// Node Operations 
bool list_append(LinkedList* list, void* data, size_t size); //여기서 size는 자료형의 사이즈. ex) int형의 사이즈 
                                                             //Q. 왜 insert_node(data)시, data의 자료형 사이즈를 같이 넣어줘야 함?
bool list_remove_at(LinkedList* list, size_t index, void** out_data); //왜 void**? 
bool list_insert_at(LinkedList* list, size_t index, void* data, size_t size);
void* list_get_at(const LinkedList* list, size_t index); //Q. 왜 void* 타입 씀? A. c에는 generic(template) 없음. void*로 모든 타입 다 받는 것. 그리고 size_t로 자료형의 사이즈를 받는 것.
bool list_set_at(LinkedList* list, size_t index, void* data, size_t size);


// Iterator functions
typedef void (*ListIteratorFn)(void* data, size_t size, void* user_data);
void list_foreach(const LinkedList* list, ListIteratorFn fn, void* user_data);


// Search Operations
ssize_t list_find(const LinkedList* list, const void* data, size_t size); // Q. what is ssize_t? diff size_t and ssize_t?

#endif
