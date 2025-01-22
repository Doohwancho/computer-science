#include <stdio.h>

int main(){
    int i = 10;
    int* pi = &i;

    int v;
    //case1)
    // v = *pi; //v에 i값을 할당
    //case2)
    // v = *pi++; //v에 i값을 할당하고, i에 포인터를 한칸(8byte) 증가
    //case3)
    // v = (*pi)++; //v에 i값을 할당하고, i에 포인터를 한칸(8byte) 증가, case2)와 똑같다.
    //case4)
    // v = *++pi; //0 - pi가 가르키는 주솟값을 8byte 증가시킴. 그 이후에 pi가 가르키는 주솟값을 읽어서 v에 할당. 
    //case5)
    v = ++*pi; //11 - 먼저 pi가 가르키는 값인 10에 +1한 후, v에 할당함.

    printf("%d\n", v);

    return 0;
}