#include <stdio.h> //printf()
#include <stdlib.h> //malloc
#include <string.h> //strlen()

int main() {
    char* str = (char*)malloc(sizeof(char) * 101);

    scanf("%s", str); //만약 int a라면, &a로 주솟값을 던져줘야 하는데, str는 이미 포인터(주솟값)이라 그냥 던져주면 된다.

    printf("%d", strlen(str));
    return 0;
}
