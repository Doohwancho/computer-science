#include <stdio.h>

int main() {
    int arr[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    int (*ptr)[4] = arr;

    printf("arr[1][2] = %d\n", *(*(ptr + 1) + 2)); //7
    printf("arr[2][3] = %d\n", *(*(arr + 2) + 3)); //12
    printf("arr[0][1] = %d\n", *(arr[0] + 1)); //2

    return 0;
}