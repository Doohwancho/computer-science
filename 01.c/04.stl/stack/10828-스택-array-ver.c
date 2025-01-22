#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define ERROR -1
#define MAX_STACK_SIZE 100001

typedef struct Stack {
    int arr[MAX_STACK_SIZE];
    int len;
} Stack;

void push(Stack*, int);
int pop(Stack*);
int size(Stack*);
int empty(Stack*);
int top(Stack*);

//push X: 정수 X를 스택에 넣는 연산이다.
void push(Stack *pstack, int a)
{
    //원래라면 100001 이상 input 왔을 때 예외처리 해야하는 자리
    pstack->arr[pstack->len] = a;
    pstack->len++;
}

//pop: 스택에서 가장 위에 있는 정수를 빼고, 그 수를 출력한다. 만약 스택에 들어있는 정수가 없는 경우에는 -1을 출력한다.
int pop(Stack *pstack)
{
    if(pstack->len == 0)
    {
        printf("%d\n", ERROR);
        return ERROR;
    }
    printf("%d\n", pstack->arr[pstack->len-1]);
    pstack->arr[pstack->len-1] = 0;
    pstack->len--;
    return pstack->len;
}

//size: 스택에 들어있는 정수의 개수를 출력한다.
int size(Stack *pstack)
{
    printf("%d\n", pstack->len);
    return pstack->len;
}

//empty: 스택이 비어있으면 1, 아니면 0을 출력한다.
int empty(Stack *pstack)
{
    if(pstack->len > 0)
    {
        printf("%d\n", 0);
        return FALSE;
    }
    else
    {
        printf("%d\n", 1);
        return TRUE;
    }
}

//top: 스택의 가장 위에 있는 정수를 출력한다. 만약 스택에 들어있는 정수가 없는 경우에는 -1을 출력한다.
int top(Stack *pstack)
{
    if(pstack->len == 0)
    {
        printf("%d\n", -1);
        return ERROR;
    }
    printf("%d\n", pstack->arr[pstack->len-1]);
    return pstack->arr[pstack->len-1];
}

int main()
{
    struct Stack stack;
    int T;
    scanf("%d\n", &T);

    stack.len = 0;

    for(int i = 0; i < T; i++)
    {
        char command[6];
        scanf("%s\n", command);

        if(strcmp(command, "push") == 0)
        {
            int a;
            scanf("%d\n", &a);
            push(&stack, a);
        } else if(strcmp(command, "pop") == 0)
        {
            pop(&stack);
        } else if(strcmp(command, "size") == 0)
        {
            size(&stack);
        } else if(strcmp(command, "empty") == 0)
        {
            empty(&stack);
        } else if(strcmp(command, "top") == 0)
        {
            top(&stack);
        }
    }
    return 0;
}
