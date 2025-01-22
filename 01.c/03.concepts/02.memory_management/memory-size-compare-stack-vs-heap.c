#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int stack_var; // Stack memory
    int* heap_var_ptr; // Heap memory

    // get the available stack memory
    int stack_free = &stack_var - (int*)sbrk(0);

    // get the available heap memory
    heap_var_ptr = (int*)malloc(sizeof(int));
    int heap_free = (int*)sbrk(0) - (int*)heap_var_ptr;

    // print the available memory
    printf("Stack memory free: %d\n", stack_free);
    printf("Heap memory free: %d\n", heap_free);

    return 0;
}


//how to run?
//gcc -o main -std=gnu99 memory-free-cost-stack-vs-heap.c


//console.log

//Stack memory free: 466906074 //466MB
//Heap memory free: 1073844208 //1.07GB



//takeaways

//1. stack memory size is fixed, heap memory size is dynamic 이라고 알고있었는데, 일단 heap size를 최소 1기가 잡아놓고, 더 필요하면 늘리는 식인건가?
//2. stack memory 얼마 안필요 할 것 같은데 466MB나 잡네? 아 이건 모든 프로그램 전체의 할당 가능한 총 stack memory size인가? heap도 매한가지고? 객체 정보 담는데 GB단위로 쓸 일이 거의 없지 않나?

//3. 아마 stack은 메모리 해제 속도 heap보다 더 빠르겠지. stack은 pop하고 array는 순차적으로 linear하게 해제시키면 되니까. heap은 주소 찾아가서 하나씩 해제시키는 거니까 더 느릴거고.
