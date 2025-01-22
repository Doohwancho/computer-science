#include <stdio.h>

int main(){
    char* p = "KOREA";

    printf("%s \n", p); //KOREA
    printf("%s \n", p+1); //OREA
    printf("%c \n", *p); //K
    printf("%c \n", *(p+3)); //E
    printf("%c \n", *p+4); //O -> Q. why O? A. K에서 아스키 +4함. K, L, M, N, O
    printf("%d \n", 'A'); //65
    printf("%d \n", 'a'); //97

    return 0;
}