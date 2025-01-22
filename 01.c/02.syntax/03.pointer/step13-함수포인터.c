#include <stdio.h>

//함수 포인터와 배열 포인터 예제

void func1(int *arr) {
    printf("func1: %d\n", *(arr + 1));
}

void func2(int (*arr)[3]) {
    printf("func2: %d\n", *(*arr + 1));
}

int main() {
    int arr[2][3] = {
        {1, 2, 3}, 
        {4, 5, 6}
    };
    void (*ptr1)(int *) = func1;
    void (*ptr2)(int (*)[3]) = func2;

    ptr1(arr[0]); //2
    ptr2(arr); //2

    return 0;
}