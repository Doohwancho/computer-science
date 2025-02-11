#include <stdio.h>

void print_binary(unsigned int value) {
    for (int i = sizeof(unsigned int) * 8 - 1; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 8 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}

int main() {
    //case1) 보수
    int num = 0b00001101; //13 in binary - 00000000 00000000 00000000 00001101
    int complement = ~num; //11111111 11111111 11111111 11110010

    printf("num: %d, complement: %d\n", num, complement); //13, -14
    printf("complement in binary: ");
    print_binary(complement); //complement in binary: 11111111 11111111 11111111 11110010

    return 0;
}