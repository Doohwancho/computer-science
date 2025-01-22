#include <stdio.h>
#include <stdlib.h>

int main() {
    int* arr; //pointer to an int
    int size = 10;
    int i;

    arr = (int*)malloc(size * sizeof(int)); //int size(4 byte) * 10개 = 40 byte만큼 arr에 할당

    for(int i = 0; i < size; i++) {
        arr[i] = i; 
        //or scanf("%d", &arr[i]); //arr[i]의 주솟값을 건내주면, 거기에 value를 넣겠다라는 뜻
        printf("%d ", arr[i]);
    }

    free(arr);

    return 0;
}