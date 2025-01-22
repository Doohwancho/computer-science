#include <stdio.h>

int main() {
    int arr1[3] = {1, 2, 3};
    int arr2[3] = {4, 5, 6};
    int *parr[2] = {arr1, arr2};  // 포인터 배열
    int (*arrp)[3] = &arr1;       // 배열 포인터

    printf("parr[0][1] = %d\n", parr[0][1]); //2
    printf("parr[1][2] = %d\n", parr[1][2]); //6
    printf("arrp[0][2] = %d\n", (*arrp)[2]); //3

    return 0;
}