#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////
// 주소록이 저장될 데이터 파일
#define DATA_FILE_NAME "Address.dat"

/////////////////////////////////////////////////////////////////////////
void ReleaseList();

/////////////////////////////////////////////////////////////////////////
typedef struct _USERDATA
{
	char szName[32];	  // 이름- hungarian, sz stands for string terminated zero
	char szPhone[32];	  // 전화번호

	struct _USERDATA *pNext; //만들 때 부터 NULL로 초기화 시키진 않구나. 약간 연역법 식으로 minimal하게 가는건가?
} USERDATA;

// 더미 헤드 노드 선언 및 정의
USERDATA g_Head = { 0 }; //global head


//입력 초기화
void flush_stdin() {
	while (getchar() != '\n'); //getchar()은 버퍼에 data 꺼내서 char단위로 보여줌
}

/////////////////////////////////////////////////////////////////////////
// 리스트에서 이름으로 특정 노드를 검색하는 함수
// return type이 USERDATA면, 함수 이름 앞에 *이 붙나 보네?
USERDATA *FindNode(char *pszName) // psz == NULL로 끝나는 문자열을 가리키는 포인터(16bit, 32bit). p = pointer, sz = string terminated zero = NULL로 끝나는 문자열
{
	USERDATA *pTmp = g_Head.pNext;
	while (pTmp != NULL) {
		if (strcmp(pTmp->szName, pszName) == 0) //같으면 결과값이 0인가 보네?
			return pTmp;

		// 다음 노드로 이동
		pTmp = pTmp->pNext;
	}

	// 일치하는 데이터를 찾지 못한 경우
	return NULL;
}

/////////////////////////////////////////////////////////////////////////
int AddNewNode(char *pszName, char *pszPhone)
{
	USERDATA *pNewUser = NULL;

	// 같은 이름이 이미 존재하는지 확인한다.
	if (FindNode(pszName) != NULL)
		return 0;

	// 메모리를 확보한다.
	pNewUser = (USERDATA*)malloc(sizeof(USERDATA)); //동적할당으로 사이즈 확보해도, 형변환을 해줘야 하는구나. security issue 때문인가? 혹여나 accidentally 다른 type이 덮어쓰면 안되니까?
	memset(pNewUser, 0, sizeof(USERDATA));

	// 메모리에 값을 저장한다.
	sprintf(pNewUser->szName, "%s", pszName);
	sprintf(pNewUser->szPhone, "%s", pszPhone);
	pNewUser->pNext = NULL;

	// 더미 노드 바로 뒤에 붙인다. 따라서 가장 최근에
	// 추가한 데이터가 가장 앞쪽에 온다.
	pNewUser->pNext = g_Head.pNext;
	g_Head.pNext = pNewUser;

	return 1;
}

/////////////////////////////////////////////////////////////////////////
// 이름을 입력받아 리스트에 추가하는 함수
void Add()
{
	char szName[32] = { 0 };
	char szPhone[32] = { 0 };

	printf("Input name : ");
	flush_stdin();
	fgets(szName, sizeof(szName), stdin);

	printf("Input phone number : ");
	fgets(szPhone, sizeof(szPhone), stdin);

	// 실제로 리스트에 추가한다.
	AddNewNode(szName, szPhone);
}

/////////////////////////////////////////////////////////////////////////
// 특정 노드를 검색하는 함수
void Search()
{
	char szName[32] = { 0 };
	USERDATA *pNode = NULL;

	printf("Input name : ");
	flush_stdin();
	fgets(szName, sizeof(szName), stdin);

	pNode = FindNode(szName);
	if (pNode != NULL) {
		printf("[%p] %s\t%s [%p]\n",
			pNode,
			pNode->szName, pNode->szPhone,
			pNode->pNext);
	}

	else {
		puts("ERROR: 데이터를 찾을 수 없습니다.");
	}

	getchar();
    flush_stdin();
}

/////////////////////////////////////////////////////////////////////////
// 리스트에 들어있는 모든 데이터를 화면에 출력하는 함수
void PrintAll()
{
	USERDATA *pTmp = g_Head.pNext;
	while (pTmp != NULL) {
		printf("[%p] %s\t%s [%p]\n", //포인터는 %p로 표현하는구남
			pTmp,
			pTmp->szName, pTmp->szPhone,
			pTmp->pNext);

		pTmp = pTmp->pNext;
	}

	getchar();
    flush_stdin();
}

/////////////////////////////////////////////////////////////////////////
// 특정 노드를 검색하고 삭제하는 함수
int RemoveNode(char *pszName)
{
	USERDATA *pPrevNode = &g_Head;
	USERDATA *pDelete = NULL;

	while (pPrevNode->pNext != NULL) {
		pDelete = pPrevNode->pNext;

		if (strcmp(pDelete->szName, pszName) == 0) {
			pPrevNode->pNext = pDelete->pNext;
			free(pDelete);

			return 1;
		}

		pPrevNode = pPrevNode->pNext;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////
// 이름을 입력받아 자료를 검색하고 삭제하는 함수
void Remove()
{
	char szName[32] = { 0 };

	printf("Input name : ");
	flush_stdin();
	fgets(szName, sizeof(szName), stdin);

	RemoveNode(szName);
}

/////////////////////////////////////////////////////////////////////////
// 메뉴를 출력하는 UI 함수
int PrintUI()
{
	int nInput = 0;

	system("clear"); // clean screen on cmd
	printf("[1] Add\t [2] Search\t [3] Print all\t [4] Remove\t [0] Exit\n:");

	// 사용자가 선택한 메뉴의 값을 반환한다.
	scanf("%d", &nInput);

	return nInput;
}

/////////////////////////////////////////////////////////////////////////
// 데이터 파일에서 노드들을 읽어와 리스트를 완성하는 함수
int LoadList(char *pszFileName)
{
	FILE *fp = fopen(pszFileName, "rb");
	USERDATA user = { 0 };

	if (fp == NULL)
		return 0;

	ReleaseList();

	while (fread(&user, sizeof(USERDATA), 1, fp))
		AddNewNode(user.szName, user.szPhone);

	fclose(fp);

	return 0;
}

/////////////////////////////////////////////////////////////////////////
// 리스트 형태로 존재하는 정보를 파일에 저장하는 함수
int SaveList(char *pszFileName)
{
	FILE *fp = fopen(pszFileName, "wb");
	USERDATA *pTmp = g_Head.pNext;


	if (fp == NULL) {
		puts("ERROR: 리스트 파일을 쓰기 모드로 열지 못했습니다.");
		getchar();
        flush_stdin();

		return 0;
	}

	while (pTmp != NULL) {
		if (fwrite(pTmp, sizeof(USERDATA), 1, fp) != 1)
			printf("ERROR: %s에 대한 정보를 저장하는데 실패했습니다.\n", pTmp->szName);

		pTmp = pTmp->pNext;
	}

	fclose(fp);

	return 1;
}

/////////////////////////////////////////////////////////////////////////
// 리스트의 모든 데이터를 삭제하는 함수
void ReleaseList()
{
	USERDATA *pTmp = g_Head.pNext;
	USERDATA *pDelete = NULL;

	while (pTmp != NULL) {
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		free(pDelete);
	}

	memset(&g_Head, 0, sizeof(USERDATA));
}

/////////////////////////////////////////////////////////////////////////
int main()
{
	int nMenu = 0;
	LoadList(DATA_FILE_NAME);

	// 메인 이벤트 반복문
	while ((nMenu = PrintUI()) != 0) {
		switch (nMenu)	{
		case 1:		  // Add
			Add();
			break;

		case 2:		  // Search
			Search();
			break;

		case 3:		 // Print all
			PrintAll();
			break;

		case 4:		  // Remove
			Remove();
			break;
		}
	}

	// 종료 전에 파일로 저장하고 메모리를 해제한다.
	SaveList(DATA_FILE_NAME);
	ReleaseList();
}
