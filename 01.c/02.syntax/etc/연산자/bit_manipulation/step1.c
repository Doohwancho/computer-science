#include <stdio.h>

int c(int v){
    printf("shit");
    printf("%d ", v);
    return 1;
}

int main() {
    int a = -2, b = !a;  //Q. what is !a? A. non zero in c is true, 1. therefore, !(-2) = false, 0
    printf("%d %d %d %d", a,b,a&&b,a||b); //-2 0 0 1
    // a in binary: 1111 1111 1111 1111 1111 1111 1111 1110
    // b in binary: 0000 0000 0000 0000 0000 0000 0000 0000
    // -2 && 0 = 0
    //Q. why a||b == 1?
    //A. either of a or b is non-zero, in this case, a is non-zero. -> 1 as true

    if(b && c(10)) { //b가 0이기에 c(10)는 동작하지 않음.
        printf("A"); 
    }
    if(b & c(20)) { //b = 0 -> c(20)에서 20이 출력됨 -> 0 & 20 = 0 -> false
        printf("B"); 
    }

    return 0;
}