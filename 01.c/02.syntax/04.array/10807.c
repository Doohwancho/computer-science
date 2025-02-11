//11
//1 4 1 2 4 2 4 2 3 4 4
//2



// array 선언시, malloc()으로 할당받기 
#include <stdio.h>
#include <stdlib.h> //malloc.h

int main() {
    int a;
    int b;
    int result = 0;
    
    scanf("%d", &a);
    //int* arr[a] = (int*)malloc((int) * a); // WRONG! 이렇게 하면 안된다.
    int* arr = (int*)malloc(a * sizeof(int)); //int* 라는 array에 첫번째 주소 포인터를 넘겨줘야 한다. 
    if(arr == NULL){
        printf("memory allocation failed \n");
        return 1;
    }
    
    for(int i = 0; i < a; i++){
        scanf("%d", &arr[i]);
    }
    
    scanf("%d", &b);
    for(int i = 0; i < a; i++){
        if(arr[i] == b) {
            result += 1;
        }
    }
    
    printf("%d\n", result);
    
    //add free()!
    free(arr);
    
    return 0;
}



//case1) 그냥 일반 답변 
#include <stdio.h>

int main() {
    int a;
//    int b;
    int c;
    int result = 0;
    scanf("%d", &a);
//    int[] arr = new array[a]; //실패!
    int arr[a];
    for(int i = 0; i < a; i++) {
        scanf("%d", &arr[i]); //그냥 바로 &arr[i] 넣으면 된다.
    }
    scanf("%d", &c);
    for(int i = 0; i < a; i++) {
        if(arr[i] == c) {
            result += 1;
        }
    }
    printf("%d", result);
    return 0;
}
