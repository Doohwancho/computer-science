#include <stdio.h>

#define ROW 9
#define COLUMN 9

int main()
{
    int a, max = -1, x,y;
    int mat[ROW][COLUMN];

    for(int i = 0; i < ROW; i++) { for(int j = 0; j < COLUMN; j++) {
        scanf("%d", &a); mat[i][j] = a;
    }}

    for(int i = 0; i < ROW; i++) { for(int j = 0; j < COLUMN; j++) {
        if(mat[i][j] > max)
        {
            max = mat[i][j];
            x = i+1;
            y = j+1;
        }
    }}
    printf("%d\n%d %d\n", max, x, y);
    return 0;
}
