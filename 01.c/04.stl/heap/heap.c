#include <stdio.h>
#include <stdlib.h>

// Structure for heap node
struct HeapNode
{
	int data;
};

// Function to get the parent node
int parent(int i)
{
	return (i-1)/2;
}

// Function to get the left child node
int left(int i)
{
	return (2*i + 1);
}

// Function to get the right child node
int right(int i)
{
	return (2*i + 2);
}

// Function to swap two nodes of the heap
void swap(struct HeapNode *x, struct HeapNode *y)
{
	struct HeapNode temp = *x;
	*x = *y;
	*y = temp;
}

// Recursive Heapify-down algorithm
// the node at index i and its two direct children
// violates the heap property
void heapify(struct HeapNode a[], int n, int i)
{
	// Get the two children of node at index i
	int l = left(i);
	int r = right(i);

	// Consider the largest among root, left child and right child
	int largest = i;
	if (l < n && a[l].data > a[i].data)
		largest = l;
	if (r < n && a[r].data > a[largest].data)
		largest = r;

	// Swap and continue heapifying if root is not largest
	if (largest != i)
	{
		swap(&a[i], &a[largest]);
		heapify(a, n, largest);
	}
}

// Function to build a Max-Heap from the given array
void buildHeap(struct HeapNode a[], int n)
{
	// Index of the last non-leaf node
	int startIdx = (n/2) -1;

	// Perform reverse level order traversal
	// from last non-leaf node and heapify
	// each node
	for (int i = startIdx; i >= 0; i--)
		heapify(a, n, i);
}

// Function to print the array
void printArray(struct HeapNode a[], int n)
{
	for (int i=0; i<n; ++i)
		printf("%d ", a[i].data);
	printf("\n");
}


// Heapsort algorithm
void heapSort(struct HeapNode a[], int n)
{
	// Build the heap
	buildHeap(a, n);

	// Perform the actual sorting
	for (int i=n-1; i>0; i--)
	{
		// Swap the root with the last element
		swap(&a[0], &a[i]);

		// Heapify the root element
		heapify(a, i, 0);
	}
}


// Driver code
int main()
{
	// Array representing Max-Heap
	// 10
	// / \
	// 5 3
	// / \
	// 2 4
	struct HeapNode a[] = {{10}, {5}, {3}, {2}, {4}};

	int n = sizeof(a)/sizeof(a[0]);

    // step1) Build the max-heap
	buildHeap(a, n);
	printArray(a, n); //10,5,3,2,4


    // step2) Sort the heap ASC order
    heapSort(a, n);
    printArray(a, n); //2,3,4,5,10

	return 0;
}
