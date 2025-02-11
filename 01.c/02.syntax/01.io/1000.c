#include <stdio.h>

int main() {
    // int* a, b; // 이건 a는 포인터, b는 그냥 int라고 한다.
    int a;
    int b;
    int result;
    scanf("%d", &a);
    scanf("%d", &b);
    result = a+b;
    printf("%d \n", result);
    return 0;
}
