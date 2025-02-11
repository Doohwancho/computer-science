#include <stdio.h>

int main() {
    int num = 10;
    int* ptr;

    ptr = &num; //assign address of num to pointer

    printf("%d\n", ptr); //num의 주솟값을 출력. 그게 num의 포인터의 value니까.
    printf("%d\n", *ptr);  //* + pointer -> pointer가 가르키는 주소의 value -> 10
    printf("%d\n", num); //num의 value를 출력. (10)
    printf("%d\n", &num); //num의 주솟값을 출력

    return 0;
}