#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE_NAME "Address.dat"

typedef struct _USERDATA
{
    char szName[32];
    char szPhone[32];

    struct _USERDATA *pLeft;
    struct _USERDATA *pRight;
} USERDATA;

USERDATA *g_Head = NULL;

void flush_stdin()
{
    while(getchar() != '\n'); //Q. 왜 EOF가 아닐까?
}

void InOrderTraversal(USERDATA *pNode)
{
    if(pNode != NULL)
    {
        InOrderTraversal(pNode->pLeft);
        printf("[%p] Name: %s, Phone: %s\n", pNode, pNode->szName, pNode->szPhone);
        InOrderTraversal(pNode->pRight);
    }
}

int AddNewNode(USERDATA **ppNode, char *pszName, char *pszPhone)
{
    if(*ppNode == NULL)
    {
        *ppNode = (USERDATA*)malloc(sizeof(USERDATA));
        if(*ppNode == NULL)
        {
            printf("Memory Allocation Failed.\n");
            return 0;
        }
        memset(*ppNode, 0, sizeof(USERDATA));
        sprintf((*ppNode)->szName, "%s", pszName);
        sprintf((*ppNode)->szPhone, "%s", pszPhone);
        (*ppNode)->pLeft = NULL;
        (*ppNode)->pRight = NULL;
        return 1;
    }

    int compareResult = strcmp(pszName, (*ppNode)->szName);

    if(compareResult < 0)
    {
        return AddNewNode(&(*ppNode)->pLeft, pszName, pszPhone);
    }
    else if(compareResult > 0)
    {
        return AddNewNode(&(*ppNode)->pRight, pszName, pszPhone);
    }
    else
    {
        printf("error! Duplicate Name!\n");
        return 0;
    }

    return 1;
}

USERDATA* SearchByName(USERDATA *pNode, const char *szName)
{
    if(pNode == NULL)
        return NULL;

    int compareResult = strcmp(szName, pNode->szName);

    if(compareResult == 0)
        return pNode;
    else if(compareResult < 0)
        return SearchByName(pNode->pLeft, szName);
    else
        return SearchByName(pNode->pRight, szName);
}

USERDATA *FindMinNode(USERDATA *pNode)
{
    if(pNode == NULL)
        return NULL;

    while(pNode->pLeft != NULL)
        pNode = pNode->pLeft;

    return pNode;
}

USERDATA *DeleteNodeByName(USERDATA *pNode, char *szInputName)
{
    if(pNode == NULL)
        return NULL;

    int compareResult = strcmp(szInputName, pNode->szName);

    if(compareResult < 0)
    {
        pNode->pLeft = DeleteNodeByName(pNode->pLeft, szInputName);
    }
    else if(compareResult > 0)
    {
        pNode->pRight= DeleteNodeByName(pNode->pRight, szInputName);

    }
    else if(compareResult == 0)
    {
        //1. right child만 있는 경우
        if(pNode->pLeft == NULL)
        {
            USERDATA *temp = pNode->pRight;
            free(pNode);
            return temp;
        }
        //2. left child만 있는 경우
        else if(pNode->pRight == NULL)
        {
            USERDATA *temp = pNode->pLeft;
            free(pNode);
            return temp;
        }

        //3. left child, right child 둘 다 있는 경우
        USERDATA *minRightSubTree = FindMinNode(pNode->pRight); //right child에 제일 작은놈 찾고
        strcpy(pNode->szName, minRightSubTree->szName); //현재 노드에 값을 덮어씌운 뒤,
        strcpy(pNode->szPhone, minRightSubTree->szPhone);
        pNode->pRight = DeleteNodeByName(pNode->pRight, minRightSubTree->szName); //recursive로 right child에 제일 작은놈으로 가서, 해당 노드의 child가 NULL이면, 오른 child | NULL반환한걸 부모노드가 받고 끝남.
    }

    return pNode;
}


int SaveInOrder(USERDATA *pNode, FILE *fp)
{
    if(pNode != NULL)
    {
        SaveInOrder(pNode->pLeft, fp);
        if(fwrite(pNode, sizeof(USERDATA), 1, fp) != 1)
            printf("ERROR: %s에 대한 정보를 저장하는데 실패했습니다.\n", pNode->szName);
        SaveInOrder(pNode->pRight, fp);
    }

    return 1;
}


void ReleasePostOrder(USERDATA *pNode)
{
    if(pNode != NULL)
    {
        ReleasePostOrder(pNode->pLeft);
        ReleasePostOrder(pNode->pRight);
        free(pNode);
    }
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
    ReleasePostOrder(g_Head);

    //4. how to parse %s and put it into char[]?
    //fread() - size_t fread(void *buffer, size_t size, size_t count, FILE *stream);
    //fread() 함수는 입력 stream에서 size 길이의 count 항목까지 읽고, 지정된 buffer에 저장합니다. 파일의 위치는 읽은 바이트의 수만큼 증가합니다.
    while(fread(&user, sizeof(USERDATA), 1, fp))
        AddNewNode(&g_Head, user.szName, user.szPhone);


    //5. missed - free *fp
    fclose(fp);

    return 0;
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

    AddNewNode(&g_Head, szName, szPhone);

    return 1;
}

int Search()
{
    char szInputName[32];
    printf("Input name: ");
    flush_stdin();
    fgets(szInputName, sizeof(szInputName), stdin);

    USERDATA *pTarget = SearchByName(g_Head, szInputName);

    if(pTarget == NULL)
        puts("ERROR: 데이터를 찾을 수 없습니다.");

    printf("[%p] %s\t%s\n", pTarget, pTarget->szName, pTarget->szPhone);
    flush_stdin();
}


void PrintAll()
{
    InOrderTraversal(g_Head);
    getchar();
    flush_stdin();
}


int Remove()
{
    char szInputName[32] = { 0 };

    printf("Input name: ");
    flush_stdin();
    fgets(szInputName, sizeof(USERDATA), stdin);
    //fgets() 함수는 현재 stream 위치에서 어느 것이 먼저 오건 첫 번째 줄 바꾸기 문자(\n)까지, 스트림의 끝까지 또는 읽은 문자 수가 n-1과 같을 때까지 문자를 읽습니다. fgets() 함수는 결과를 string에 저장하고 스트링 끝에 널(null) 문자(\0)를 추가합니다. string은 줄 바꾸기 문자를 포함합니다(읽은 경우). n이 1이면 string이 비어 있습니다.

    USERDATA *pDelete = DeleteNodeByName(g_Head, szInputName);

    if(pDelete == NULL)
    {
        printf("입력한 name이 데이터베이스에 존재하지 않습니다.\n");
        return 0;
    }


    return 1;
}



//save g_Head into FILE
int Save(char *pszFileName)
{
    //1. open file
    FILE *fp = fopen(pszFileName, "wb");
    USERDATA *pHead = g_Head;

    //2. null check for open file
    if(fp == NULL)
    {
        puts("ERROR: 리스트 파일을 쓰기 모드로 열지 못했습니다."); //puts() 함수는 지정된 string을 표준 출력 스트림 stdout에 씁니다. 또한 새 행 문자를 출력에도 추가합니다. 끝 널 문자가 작성되지 않습니다.
        //getchar();
        flush_stdin();
        return 0;
    }

    //3. insert g_Head into file
    if(!SaveInOrder(g_Head, fp))
    {
        fclose(fp);
        return 0;
    }

    fclose(fp);

    return 1;
}

int main()
{
    int nMenu = 0;
    LoadList(DATA_FILE_NAME);

    //main event loop
    while((nMenu = PrintUI()) != 0) //GUI
    {
        switch(nMenu) { //event input
            case 1:
                Add();
                break;
            case 2:
                Search();
                break;
            case 3:
                PrintAll();
                break;
            case 4:
                Remove();
                break;
        }
    }

    Save(DATA_FILE_NAME);
    ReleasePostOrder(g_Head);

    return 0;
}
