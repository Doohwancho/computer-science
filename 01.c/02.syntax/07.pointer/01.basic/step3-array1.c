#include <stdio.h>
#include <stdlib.h>

int main() {
    int* arr; //int size pointer. arr[0], arr[1] 읽을 때 int byte size만큼 끊어서 읽는다.
    int size = 10;
    int i;

    arr = (int*)malloc(size * sizeof(int)); //int size(4 byte) * 10개 = 40 byte만큼 arr에 할당

    for(int i = 0; i < size; i++) {
        arr[i] = i; 
        //or scanf("%d", &arr[i]); //arr[i]의 주솟값을 건내주면, 거기에 value를 넣겠다라는 뜻
        printf("%d ", arr[i]);
    }

    free(arr); //malloc()으로 메모리 할당한걸 해제해준다. 

    return 0;
}
