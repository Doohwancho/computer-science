#include <stdio.h>

int main() {
    char a[4][10] = {"ABCD", "EFGH", "IJKL", "MNOP"};
    char* p = *a;
    int s = sizeof(p) / sizeof(p[0]); //sizeof(pointer) = 8, sizeof(char) = 1 -> 8/1 = 8

    printf("Size of s: %d\n", s); //8
    printf("Size of s: %d\n", sizeof(p)); //8. 왜? p = pointer(char) -> 모든 포인터는 8byte, therefore 8
    printf("Size of s: %d\n", sizeof(p[0])); //1. 왜? p[0] = char -> 1byte

    for(int i = 0; i < s; i++) {
        printf("%c", p[i]); //ABCD NULL NULL NULL NULL
    }

    return 0;
}