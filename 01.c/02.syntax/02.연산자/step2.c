#include <stdio.h>

int main() {
    int a = 12, b = 5, sum = 2; 
    b *= a /= 4; //3 15 2
    sum += ++a * b-- /4; // *, / 있을 땐 걍 순서대로 연산함 -> sum + 15 = 17

    printf("%d", sum); //14 WRONG! ->  17

    return 0;
}