#include <stdio.h>

struct hello {
    int key;
};

struct hello a[3][4] = {
    27,43,87,32,
    12,51,21,89,
    80,40,44,99
}, (*p)[4], *q; //q is pointer to whole 2중 어레이, p is pointer to 1차 어레이

int main(){
    p = a;
    q = a[0];
    p++;
    q++;
    printf("%d", (*p)->key + q->key); //12 + 43 = 55

    return 0;
}