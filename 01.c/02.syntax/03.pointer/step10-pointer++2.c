#include <stdio.h>

int main() {
    int a = 5, b = 10, c = 15;
    int *ptr;

    ptr = &a;
    //case1)
    // *ptr++ = 100; //이렇게 하면 정상적으로 a에 100이 들어감. ++은 100 넣은 이후에 실행됨.
    //case2)
    *(ptr++) = 100; //이렇게 하면 정상적으로 a에 100이 들어감. ++은 100 넣은 이후에 실행됨.
    //Q. 왜 case2도 a = 100이 할당됨? a = 5 이어야 하지 않음?
    //난 실행순서가, ptr++이 먼저하고, 그 다음에 *ptr = 100이 실행되는 줄 알았음.
    //근데 그게 아님. ptr = 100이 실행되고, 그 다음에 ptr++이 실행됨.
    //case3) what if?
    // printf("a: %d, b: %d, c: %d, ptr: %d\n", a, b, c, *ptr); //a = 5, b = 10, c = 15, ptr = 5
    // *(++ptr) = 100; //ptr이 a 주솟값 가지고 있었는데,
                       //++ptr 하는 순간 a 주솟값 아님. 그래서 100 넣어도, a값이 바뀌지 않은 것.
    // printf("a: %d, b: %d, c: %d, ptr: %d\n", a, b, c, *ptr); //a = 5, b = 10, c = 15, ptr = 100

    ptr = &b;
    (*ptr)++;
    printf("a: %d, b: %d, c: %d, ptr: %d\n", a, b, c, *ptr); //a = 5, b = 11, c = 15, ptr = 11

    ptr = &c;
    ++*ptr;

    printf("a: %d, b: %d, c: %d\n", a, b, c); //a = 100, b = 11, c = 16

    return 0;
}
