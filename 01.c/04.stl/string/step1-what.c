#include <stdio.h>
#include <string.h>

int main() {
    char str1[] = "Hello";
    char str2[50];
    char str3[50];

    //case1) strcpy - copying a string
    strcpy(str2, str1);
    printf("%s\n", str2); //Hello

    //case2) strcmp - compare strings
    if (strcmp(str1, str2) == 0) {
        printf("str1 and str2 are equal.\n"); //str1 and str2 are equal.
    } else {
        printf("str1 and str2 are not equal.\n");
    }

    //case3) 원본까지 변경되지 않음.
    strcpy(str1, "World");
    printf("%s\n", str1); //World
    printf("%s\n", str2); //Hello

    //case4) strlen - length of string
    printf("length of str1: %d\n", strlen(str1)); //5

    //case5) strcat - concatenation
    strcat(str2, " World!");
    printf("%s\n", str2); //Hello World!

    //case6) strchr - finding a character in a string
    char* p = strchr(str2, 'o'); //o가 처음 나오는 위치를 가르킴.
    if(p!= NULL) {
        printf("o is found at %d\n", p - str2); //o is found at 4
    } else {
        printf("o is not found.\n");
    }

    return 0;
}