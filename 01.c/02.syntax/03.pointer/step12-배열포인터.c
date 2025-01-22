#include <stdio.h>

int main() {
    int arr[2][3] = {1,2,3,
                     4,5,6};
    int (*p)[3] = NULL;  //주의! 2차원 배열이 아님. 그냥 1차원 배열에 대한 주솟값을 가리킴.
    p = arr; //{1,2,3,
             // 4,5,6}

    //팁: [] 한번당 * 한번임.
    printf("%d\n", *(p[1])); //4(value) 
    printf("%d\n", *(p[1]+2)); //6(value)
    printf("%d\n", p[1][2]); //6(value)
    printf("%d\n", p[0+2]); //error - 다른 값
    printf("%d\n", *(p[0]+1) + *(p[1]+2)); //Q. *(2)+ *(4) = ?  A. p[0]은 1이 아니라 첫번째 array의 주솟값이래~ -> 2+6 = 8
    
    printf("%d\n", p[0]); //1이 아니라 주솟값을 프린트함
    printf("%d\n", *p[0]); //1
    printf("%d\n", p[0][2]); //3
    
    printf("%d\n", p[1]); //4의 주솟값
    printf("%d\n", *(p+1)); //4의 주솟값 Q. 왜? A. p는 이중 배열에 대한 주솟값이고, *(p+1)은 2차원 배열 두번째 배열에 대한 주솟값
    printf("p: %d\n", p); //1의 주소. 1805019840
    printf("*p: %d\n", *p); //1의 주소. 1805019840
    printf("**p: %d\n", **p); //1
    
    printf("%d\n", *p[1]); //4,  [n] 한번 하는게 *한번 하는거랑 같은 듯?
    printf("%d\n", **(p+1)); //4
    
    printf("%d\n", *(*(p+1)+0) + *(*(p+1)+1)); //*(103번지 주소) *(103번지 주소)+1 = 4+5 = 9

    return 0;
}