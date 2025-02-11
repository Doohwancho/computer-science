#include <stdio.h>

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int* ptr = arr; // 포인터를 배열의 첫 번째 요소에 대한 포인터로 설정

    printf("Value at ptr: %d\n", *ptr); // 1
    printf("Value at ptr++: %d\n", *ptr++); // 1 - ptr을 증가시키고, 그 이전 위치의 값을 출력
    printf("Value at ptr: %d\n", *ptr); // 2 (이전에 ptr++한게 지금 반영됨)
    printf("Value at ++ptr: %d\n", *++ptr); // 3 - ptr을 증가시키고, 증가된 위치의 값을 출력

    return 0;
}
