#include <stdio.h>
#include <string.h>

// strcpy을 구현해보자
// char	*strcpy(char *__dst, const char *__src);
char* cstrcpy(char* dist, const char* src) {
    int i = 0;
    while(src[i]!= '\0') {
        dist[i] = src[i];
        i++;
    }
    dist[i] = '\0';

    return dist;
}

int main() {
    char str1[20] = "Hello World!";
    char str2[20];

    cstrcpy(str2, str1);
    printf("%s\n", str2); //Helo World!

    return 0;
}