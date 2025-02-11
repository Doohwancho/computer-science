#include <stdio.h>

// Function to swap two integers using call by value
void swapByValue(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}

// Function to swap two integers using call by reference
void swapByReference(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 10, y = 20;

    printf("Before swapping:\n");
    printf("x = %d, y = %d\n", x, y);

    // Call by value
    swapByValue(x, y);
    printf("After swapping by value:\n");
    printf("x = %d, y = %d\n", x, y); // No change in values of x and y

    // Call by reference
    swapByReference(&x, &y);
    printf("After swapping by reference:\n");
    printf("x = %d, y = %d\n", x, y); // Values of x and y are swapped

    return 0;
}
