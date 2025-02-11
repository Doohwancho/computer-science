#include <stdio.h>
#include <stdlib.h>

void initialize(int* arr, int size) {
    for(int i = 0; i < size; i++) {
        arr[i] = i;
    }
}

void printArr(int* arr, int size) {
    for(int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void sumArr(int* arr, int size) {
    int sum = 0;
    for(int i = 0; i < size; i++) {
        sum += arr[i];
    }
    printf("%d\n", sum);
}

int main() {
    int size = 10;
    int* arr;

    arr = (int*)malloc(size * sizeof(int));

    initialize(arr, size);

    printArr(arr, size);

    sumArr(arr, size);

    free(arr);

    return 0;
}