#include <stdio.h>

int a = 1, b = 2, c = 3;

int f() {
    int b,c;
    a=b=c=4; //b,c는 로컬 변수 접근하니까 그걸 쓰고, a는 로컬에 없으니 전역변수 쓰고.
    return a+b+c;
}

int main() {
    printf("%3d \n", f()); // 12
    printf("%3d%3d%3d \n", a,b,c); //4,2,3
    return 0;
}