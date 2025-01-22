#include <stdio.h>
#include <string.h>

// strcat을 구현해보자
// char	*strcat(char *__s1, const char *__s2);
char* cstrcat(char* s1, const char* s2) {
   char* head = s1;
   while(head != '\0') {
    head++;
   }
   while(*s2!= '\0') {
       *head = *s2;
       head++;
       s2++;
   }
   return head;
}

int main() {
    char str1[20] = "Hello World!";

    char* newStr = strcat(str1, "!@#$");
    printf("%s\n", newStr); 

    return 0;
}