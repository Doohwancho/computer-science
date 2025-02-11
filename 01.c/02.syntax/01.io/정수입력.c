#include <stdio.h>

int main() {
    int a;
    int b;
    int result;
    scanf("%d", &a); //변수 a의 주솟값을 넘겨준다.
    scanf("%d", &b); //변수 b의 주솟값을 넘겨준다.

    result = a+b;
    printf("%d \n", result); //그냥 printf(result);가 아니라, 타입을 표시해줘야 한다.
    return 0;
}
