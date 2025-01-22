#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 20

// Node of linked list
struct Node
{
    char* key;
    int value;
    struct Node* next; //얘 역시 hash 겹쳤을 때, 다음 노드에 넣게 linkedlist 구조로 되어있음.
};

// Hash table
struct Hash
{
    struct Node* head;
    int size; //linked list처럼 node를 .next로 연결하는데, 얜 size까지 있네?
};

// Function to create a hash table of given size
struct Hash* createHash(int size)
{
    struct Hash* hash = (struct Hash*) malloc(sizeof(struct Hash));
    hash->size = size; //20

    // Create and initialize head of all lists
    hash->head = (struct Node*) malloc(sizeof(struct Node)*size); //20개의 node를 만들어서 head에 넣음. 각 node는 linkedlist이고, 각 node를 잇는 것 역시 linkedlist로 되어있다고 보면 되네.
    int i;
    for (i=0; i<size; i++)
        hash->head[i].next = NULL;

    return hash;
}

// Function to calculate index using key
int hashCode(struct Hash* hash, char* key)
{
    int index = 0;
    int i;
    for (i=0; i<strlen(key); i++)
        index += (int)key[i];
    return index % hash->size;
}

// Function to search
struct Node* search(struct Hash* hash, char* key)
{
    // Calculate index
    int index = hashCode(hash, key);

    // search the list at that specific index
    struct Node* head = hash->head[index].next;
    while (head)
    {
        if (strcmp(head->key, key) == 0) //strings are equal
            return head;
        head = head->next; //이전에 겹치는 hash index에 넣었던 node를 .next로 연결해놨으니까, 다음 node로 넘어가면서 key값을 찾는다.
    }
    return NULL;
}

// Function to insert
void insert(struct Hash* hash, char* key, int value)
{
    struct Node* temp = search(hash, key);
    if (temp == NULL)
    {
        // Create a new node
        struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
        newNode->key = key;
        newNode->value = value;

        // Calculate index of new node
        int index = hashCode(hash, key);

        // Insert new node at beginning
        newNode->next = hash->head[index].next;
        hash->head[index].next = newNode;
    }
    else // Just update the value
        temp->value = value;
}

// Driver code
int main()
{
    char keys[][8] = {"the", "hello", "there"};
    int value[] = {3, 4, 5};

    int n = sizeof(keys)/sizeof(keys[0]); //3

    // Create hash table
    struct Hash* hash = createHash(SIZE);

    // Insert keys into hash table
    int i;
    for (i=0; i<n; i++)
        insert(hash, keys[i], value[i]); //String인 key를 받아 char로 바꾸고 (int)해서 index를 구한 후, % hash->size 해서 해쉬로 인덱스 구한다. 해당 인덱스 장소에 value를 넣는거고.

    // Search for different keys
    printf("%d\n", search(hash, "the")->value);
    printf("%d\n", search(hash, "hello")->value);
    printf("%d\n", search(hash, "there")->value);

    return 0;
}
