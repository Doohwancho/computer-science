#include <stdio.h>
#include <string.h>

//strlen을 구현해보자

size_t cstrlen(const char* str) {
    size_t len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}

int main() {
    char str1[] = "Hello World!";

    int len = cstrlen(str1);
    printf("%d\n", len); //5

    return 0;
}