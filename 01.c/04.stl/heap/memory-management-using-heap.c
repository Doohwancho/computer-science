#include <stdlib.h>
#include <stdio.h>

// function to create and initialize a heap of given capacity
int *createAndInit(int capacity)
{
    // Allocate memory for the heap
    int *heap = (int*)malloc(capacity * sizeof(int));

    // Initialize the heap with default values
    for (int i = 0; i < capacity; i++)
        heap[i] = 99999;

    // Return pointer to the heap
    return heap;
}

// function to insert a node into the heap
void insertNode(int *heap, int capacity, int data)
{
    // Check if heap is full
    if (capacity == 0)
        return;

    // Insert the new node to the end of the heap  -------------> step1) 맨 마지막에 삽입. 맨 마지막 node가 leaf인가 봄.
    heap[capacity-1] = data;

    // Heapify the new node
    int current = capacity-1;
    while (current != 0 && heap[(current-1)/2] > heap[current]) // step2) 부모와 비교해서 부모보다 더 작으면, 부모와 swap 하는걸 recursive -> 결국 제일 작은 애가 root node로 감. 이경우 index 0
    {
        // Swap the two nodes
        int temp = heap[(current-1)/2];
        heap[(current-1)/2] = heap[current];
        heap[current] = temp;

        // Move to the parent node
        current = (current-1)/2;
    }
}

// Driver code
int main()
{
    // Create and initialize a heap
    int capacity = 10;
    int *heap = createAndInit(capacity);

    // Insert nodes into the heap
    insertNode(heap, capacity, 3);
    insertNode(heap, capacity, 4);
    insertNode(heap, capacity, 9);
    insertNode(heap, capacity, 5);
    insertNode(heap, capacity, 2);

    // Print the nodes of the heap
    printf("Heap elements:\n");
    for (int i = 0; i < capacity; i++)
        printf("%d ", heap[i]);

    return 0;
}


//console.log

//Heap elements:
//2 3 99999 99999 4 99999 99999 99999 99999 5 %

//3,4,9,5,2 넣었는데, 9가 씹힌것 빼고 나름 양호하게 들어간 듯
//9가 씹힌 이유는, 9가 2타자로 3타자로 들어갔을 때, 처음에 leaf노드에서 탈출해서 index4번에 갔는데, 그 다음타자 5번이 덮어쓰기해서 그런 듯?

//이 heap은 트리가 아니라 linear array라 사이즈를 다 활용하지 못하고 logn만큼만 활용하는 듯

//heap을 쓰는 이유가, 가장 큰 값이나 작은 값이 root node에 있어서, 그 값을 빠르게 찾기 위해서인데, 이게 왜 memory allocation, deallocation이랑 관련있는거지?


//이건 tsoding이 memory management 구현할 때, 다시 봐야 할 듯.
