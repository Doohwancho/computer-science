#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define ERROR -1

typedef struct Queue {
    int front;
    int rear;
    int *q;
} Queue;

Queue* createQueue(int);
void push(Queue*, int);
int pop(Queue*);
int size(Queue*);
int empty(Queue*);
int front(Queue*);
int back(Queue*);

Queue* createQueue(int maxSize)
{
    Queue* pReturn = (Queue*)malloc(sizeof(Queue));
    pReturn->q = (int*)malloc(sizeof(int) * (maxSize+1));
    pReturn->front = 0;
    pReturn->rear = 0;
    memset(pReturn->q, 0, sizeof(int) * (maxSize+1));

    return pReturn;
}

//push X: 정수 X를 큐에 넣는 연산이다.
void push(Queue *pQ, int data)
{
    pQ->q[pQ->rear++] = data;
}

//pop: 큐에서 가장 앞에 있는 정수를 빼고, 그 수를 출력한다. 만약 큐에 들어있는 정수가 없는 경우에는 -1을 출력한다.
int pop(Queue *pQ)
{
    if(empty(pQ))
    {
        return -1;
    }
    int ret = pQ->q[pQ->front];
    pQ->q[pQ->front++] = 0;
    return ret;
}

//size: 큐에 들어있는 정수의 개수를 출력한다.
int size(Queue *pQ)
{
    return pQ->rear-pQ->front;
}

//empty: 큐가 비어있으면 1, 아니면 0을 출력한다.
int empty(Queue *pQ)
{
    if(size(pQ) == 0) return 1;
    else return 0;
}

//front: 큐의 가장 앞에 있는 정수를 출력한다. 만약 큐에 들어있는 정수가 없는 경우에는 -1을 출력한다.
int front(Queue *pQ)
{
    if(empty(pQ)) return -1;
    return pQ->q[pQ->front];
}

//back: 큐의 가장 뒤에 있는 정수를 출력한다. 만약 큐에 들어있는 정수가 없는 경우에는 -1을 출력한다.
int back(Queue *pQ)
{
    if(empty(pQ)) return -1;
    return pQ->q[pQ->rear-1];
}

int main()
{
    int T;
    scanf("%d", &T);

    Queue* pQ = createQueue(T); //주의! 백준 10845 문제 한정 input 나오는 갯수만큼 넣게끔 커스텀한 것

    for(int i = 0; i < T; i++)
    {
        char command[6];
        scanf("%s", command);

        if(strcmp(command, "push") == 0)
        {
            int a;
            scanf("%d", &a);
            push(pQ, a);
        } else if(strcmp(command, "pop") == 0)
        {
            printf("%d\n", pop(pQ));
        } else if(strcmp(command, "size") == 0)
        {
            printf("%d\n", size(pQ));
        } else if(strcmp(command, "empty") == 0)
        {
            printf("%d\n", empty(pQ));
        } else if(strcmp(command, "front") == 0)
        {
            printf("%d\n", front(pQ));
        } else if(strcmp(command, "back") == 0)
        {
            printf("%d\n", back(pQ));
        }
    }
    return 0;
}
