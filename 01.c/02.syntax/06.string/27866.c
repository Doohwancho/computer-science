#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// solution1) fgets()
int main() {
    char str[1001];
    int a;

    // Input string 
    fgets(str, sizeof(str), stdin); //fgets()는 맨 끝에 '\n'를 강제로 넣는다. 

    // Remove '\n' at the end
    size_t len = strlen(str);
    if(len > 0 && str[len - 1] == '\n') {
        str[len-1] = '\0'; //따라서 string으로 바꾸려면, 맨 끝 '\n'을 '\0'으로 바꿔줘야 한다.
    }

    // input the index
    scanf("%d", &a);

    //print the string
    printf("%c\n", str[a-1]);

    return 0;
}


// solution2) scanf()
int main() {
    char str[1001];
    int a;

    scanf("%s", &str); //scanf() automatically adds '\0' at the end.
    scanf("%d", &a);

    printf("%c\n", str[a-1]);
    return 0;
}


// solution3) malloc()
int main(){
    char* str = (char*)malloc(1001 * sizeof(char));
    int a;

    if(str == NULL){
        printf("memory allocationf failed!\n");
        return 1;
    }

    scanf("%s", str);
    scanf("%d", &a);

    printf("%c\n", str[a-1]);

    free(str);
    return 0;
}
