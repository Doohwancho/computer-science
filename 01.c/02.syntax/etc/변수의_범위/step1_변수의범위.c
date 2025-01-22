#include <stdio.h>

int star = 10;

void printStar() {
    printf("%d\n", star); //10 -> {} 안에 선언된 star가 없으면 전역변수에서 star를 찾는다. main(){}의 컨텍스트와는 별개다.
}

int main() {
    int star = 5;
    printStar();
    printf("%d\n", star); //5

    return 0;
}