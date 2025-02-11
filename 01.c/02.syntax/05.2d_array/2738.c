#include <stdio.h>
#include <stdlib.h>

int main() {
    int x, y;
    scanf("%d", &x);
    scanf("%d", &y);
    
    // int a[x][y];
    // int b[x][y];

    //2d array how? for a, b?
    // int** a = (int**)malloc(x * y * sizeof(int));
    int** a = (int**)malloc(sizeof(int*) * x);
    for(int i = 0; i < x; i++) {
        a[i] = (int*)malloc(sizeof(int) * y);
    }

    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    // int** b = (int**)malloc(x * y * sizeof(int));
    int** b = (int**)malloc(sizeof(int*) * x);
    for(int i = 0; i < x; i++) {
        b[i] = (int*)malloc(sizeof(int) * y);
    }

    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            scanf("%d", &b[i][j]);
        }
    }

    //add a+b 더하기 
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            a[i][j] += b[i][j];
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    

    //memory 해제 
    for(int i = 0; i < x; i++) {
        free(a[i]);
    }
    free(a);
    for(int i = 0; i < x; i++) {
        free(b[i]);
    }
    free(b);

    return 0;
}
