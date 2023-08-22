#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE_NAME "Address.dat"

void ReleaseList(); //Q. why forward declare only ReleaseList() functions? A. c는 pop인데, LoadList()에서 아직 declared 되지 않은 ReleaseList() 써야하니까, 컴파일러한테 선언했다고 알려주는 것.
int PrintUI();
int Add();
void PrintAll();

typedef struct _USERDATA
{
    char szName[32];
    char szPhone[32];

    struct _USERDATA *pNext;
} USERDATA;

USERDATA g_Head = { 0 };

void flush_stdin()
{
    while(getchar() != '\n'); //Q. 왜 EOF가 아닐까?
}

int AddNewNode(char *pszName, char *pszPhone)
{
    //1. create empty node for insert
    USERDATA *pNewNode = NULL;

    //2. missed - 같은 이름의 노드가 이미 존재하는지 확인

    //3. insert value(name, phone) into newNode
    /* pNewNode->szName = szName; */ //Q. why wrong? A. 메모리 확보가 안된 NULL된 상태에서 값 넣으면 안되고, memset()으로 동적 메모리 할당받아야 값을 넣든가 말든가 할 수 있음
    /* pNewNode->szPhone = szPhone; */
    /* pNewNode->pNext = NULL; */

    //missed - 3-1. 메모리 공간 확보
    pNewNode = (USERDATA*)malloc(sizeof(USERDATA));

    //missed - 3-2. 메모리 초기화
    memset(pNewNode, 0, sizeof(USERDATA));

    //missed - 3-3. 메모리에 값을 저장
    sprintf(pNewNode->szName, "%s", pszName); //그냥 pNewNode->szName = szName; 으로 넣는게 아니라, sprintf(); 로 값을 넣는구나.
    sprintf(pNewNode->szPhone, "%s", pszPhone);
    pNewNode->pNext = NULL;


    //g_Head에서 node->pNext == NULL일 떄까지 while로 가서
    /* USERDATA *head = &g_Head; */ //Q. &가 맞나?
    /* while(head->pNext != NULL) head = head->pNext; */
    /* head->pNext = pNewNode; */
    pNewNode->pNext = g_Head.pNext;
    g_Head.pNext = pNewNode;

    return 1;
}
int LoadList(char *pszFileName)
{
    //1. open file with filename
    FILE *fp = fopen(pszFileName, "rb");
    USERDATA user = { 0 };

    //2. missed - null check *fp
    if(fp == NULL)
        return 0;

    //3. missed - zero-initialization g_Head;
    ReleaseList();

    //4. how to parse %s and put it into char[]?
    //fread() - size_t fread(void *buffer, size_t size, size_t count, FILE *stream);
    //fread() 함수는 입력 stream에서 size 길이의 count 항목까지 읽고, 지정된 buffer에 저장합니다. 파일의 위치는 읽은 바이트의 수만큼 증가합니다.
    while(fread(&user, sizeof(USERDATA), 1, fp))
        AddNewNode(user.szName, user.szPhone);


    //5. missed - free *fp
    fclose(fp);

    return 0;
}


void ReleaseList()
{
    //1. g_Head->pNext && pDelete 만듬
    USERDATA *pTmp = g_Head.pNext;
    USERDATA *pDelete = NULL;

    //2. g_Head->pNext를 while로 돌면서 free(pDelete)
    while(pTmp != NULL)
    {
        pDelete = pTmp;
        pTmp = pTmp->pNext;

        free(pDelete);
    }

    //3. free(g_Head)
    memset(&g_Head, 0, sizeof(USERDATA));
}

int PrintUI()
{
    int nInput = 0;
    system("clear"); //clear screen on command
    printf("[1] Add\t [2] Search\t [3] Print all\t [4] Remove\t [0] Exit\n:");

    scanf("%d", &nInput);
    return nInput;
}

int Add()
{
    //1. pNewNode를 만든다
    USERDATA *pNewNode = NULL;

    //missed - char szName[32], szPhone[32] 생성
    char szName[32] = { 0 };
    char szPhone[32] = { 0 };

    //missed - clear stdin
    flush_stdin();

    //missed - fgets()로 입력값을 szName, szPhone으로 받아옴
    printf("Input name: ");
    fgets(szName, sizeof(szName), stdin);

    printf("Input phone number: ");
    fgets(szPhone, sizeof(szPhone), stdin);

    AddNewNode(szName, szPhone);

    return 1;
}

void PrintAll()
{
    //get g_Head
    USERDATA *pHead = g_Head.pNext; //Q. 맨 첫 노드엔 안넣는 이유는?

    flush_stdin();

    //while loop 돌면서 print
    while(pHead != NULL)
    {
        printf("[%p] %s\t%s [%p]\n", pHead, pHead->szName, pHead->szPhone, pHead->pNext);
        pHead = pHead->pNext;
    }

    //clear buffer
    getchar();
    flush_stdin();
}

//save g_Head into FILE
int SaveList(char *pszFileName)
{
    //1. open file
    FILE *fp = fopen(pszFileName, "wb");
    USERDATA *pHead = g_Head.pNext;

    //2. null check for open file
    if(fp == NULL)
    {
        puts("ERROR: 리스트 파일을 쓰기 모드로 열지 못했습니다."); //puts() 함수는 지정된 string을 표준 출력 스트림 stdout에 씁니다. 또한 새 행 문자를 출력에도 추가합니다. 끝 널 문자가 작성되지 않습니다.
        //getchar();
        flush_stdin();
        return 0;
    }

    //3. insert g_Head into file
    while(pHead != NULL)
    {
        if(fwrite(pHead, sizeof(USERDATA), 1, fp) != 1)
            printf("ERROR: %s에 대한 정보를 저장하는데 실패했습니다.\n", pHead->szName);

        pHead = pHead->pNext;
    }

    //4. close file
    fclose(fp);

    return 1;
}

int main()
{
    int nMenu = 0;
    LoadList(DATA_FILE_NAME);

    //main event for loop
    while((nMenu = PrintUI()) != 0)
    {
        switch(nMenu) {
            case 1:
                Add();
                break;
            case 3:
                PrintAll();
                break;
        }
    }

    SaveList(DATA_FILE_NAME);
    ReleaseList();

    return 0;
}
