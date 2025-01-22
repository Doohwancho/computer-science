#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define ERROR -1
#define QUEUE_MAX_SIZE 10002

typedef struct Queue {
    int arr[QUEUE_MAX_SIZE];
    int size;
} Queue;

void push(Queue*, int);
int pop(Queue*);
int size(Queue*);
bool empty(Queue*);
int front(Queue*);
int back(Queue*);

//push X: 정수 X를 큐에 넣는 연산이다.
void push(Queue *pqueue, int a)
{
    if (pqueue->size >= QUEUE_MAX_SIZE - 1) {
        printf("%d\n", ERROR);
        return;
    }
    pqueue->arr[pqueue->size] = a;
    pqueue->size++;
}

//pop: 큐에서 가장 앞에 있는 정수를 빼고, 그 수를 출력한다. 만약 큐에 들어있는 정수가 없는 경우에는 -1을 출력한다.
int pop(Queue *pqueue)
{
    if(pqueue->size == 0)
    {
        printf("%d\n", -1);
        return ERROR;
    }
    int ret = pqueue->arr[0]; //Q. call by reference면, step2에서 앞으로 한칸 땡길 때, ret값이 바뀔텐데..? call by value이면 괜찮겠지만서도..

    for(int i = 0; i < pqueue->size; i++)
    {
        pqueue->arr[i] = pqueue->arr[i+1];
    }

    pqueue->arr[pqueue->size-1] = 0;
    pqueue->size--;

    printf("%d\n", ret); //?
    return ret;
}

//size: 큐에 들어있는 정수의 개수를 출력한다.
int size(Queue *pqueue)
{
    printf("%d\n", pqueue->size);
    return pqueue->size;
}

//empty: 큐가 비어있으면 1, 아니면 0을 출력한다.
bool empty(Queue *pqueue)
{
    if(pqueue->size == 0)
    {
        printf("%d\n", 1);
        return TRUE;
    }
    else
    {
        printf("%d\n", 0);
        return FALSE;
    }
}

//front: 큐의 가장 앞에 있는 정수를 출력한다. 만약 큐에 들어있는 정수가 없는 경우에는 -1을 출력한다.
int front(Queue *pqueue)
{
    if(pqueue->size == 0)
    {
        printf("%d\n", -1);
        return ERROR;
    }
    printf("%d\n", pqueue->arr[0]);
    return pqueue->arr[0];
}

//back: 큐의 가장 뒤에 있는 정수를 출력한다. 만약 큐에 들어있는 정수가 없는 경우에는 -1을 출력한다.
int back(Queue *pqueue)
{
    if(pqueue->size == 0)
    {
        printf("%d\n", -1);
        return ERROR;
    }
    printf("%d\n", pqueue->arr[pqueue->size-1]);
    return pqueue->arr[pqueue->size-1];
}

int main()
{
    struct Queue queue;
    int T;
    scanf("%d", &T);

    queue.size = 0;

    for(int i = 0; i < T; i++)
    {
        char command[6];
        scanf("%s", command);

        if(strcmp(command, "push") == 0)
        {
            int a;
            scanf("%d", &a);
            push(&queue, a);
        } else if(strcmp(command, "pop") == 0)
        {
            pop(&queue);
        } else if(strcmp(command, "size") == 0)
        {
            size(&queue);
        } else if(strcmp(command, "empty") == 0)
        {
            empty(&queue);
        } else if(strcmp(command, "front") == 0)
        {
            front(&queue);
        } else if(strcmp(command, "back") == 0)
        {
            back(&queue);
        }
    }
    return 0;
}
