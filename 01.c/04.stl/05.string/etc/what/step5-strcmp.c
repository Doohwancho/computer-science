#include <stdio.h>
#include <string.h>

// cstrcmp을 구현해보자
int cstrcmp(const char* str1, const char* str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 < *str2) {
            return -1;
        } else if (*str1 > *str2) {
            return 1;
        }
        str1++;
        str2++;
    }
    if (*str1 == '\0' && *str2 == '\0') {
        return 0;
    } else if (*str1 == '\0') {
        return -1;
    } else {
        return 1;
    }
}

int main() {
    char str1[20] = "Hello World!";
    char str2[20] = "Hello World!";

    int same = cstrcmp(str1, str2);
    printf("%d\n", same); //0

    return 0;
}