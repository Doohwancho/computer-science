#include <stdio.h>
#include <string.h>

int main()
{
	/*
	//문자열 : 문자들의 열거
	char arr[] = "abc";
	
	printf("%s\n", arr);

	printf("배열의 크기: %d\n", sizeof(arr) / sizeof(char));
	//배열 안 원소들이 3개인데 배열의 크기가 4라고 나오는 이유는?
	//이유
	//char arr[] = {'a','b','c','\0'}; 으로 선언되서 그럼.
	//\0은 null문자임. 여기가 바로 문자열의 끝이다 라는 것을 나타냄
	//따라서 \0이 중간에 나오면 그 뒤에 문자들은 처리하지 않음
	
	char s[1000];

	scanf_s("%s", s, 1000); 
	//s앞에는 &을 붙이지 않는다.
	//뒤의 배열의 크기를 같이 적어주어야 한다
	//왜?.
	//왜냐하면 scanf_s함수와 같이 _s가 붙은 함수들은 문자열을 받을 때 
	//사용자가 입력한 문자의 갯수가 메모리의 크기를 넘어서는 문제를 막기위해 
	//사용할 메모리의 크기를 명시해야 하기 때문.
	
	printf("%s\n", s);
	

	//문자열에 관련된 4가지 함수
	//stdio.h처럼 문자열에 관련된 함수를 쓸때도, 새로운 파일을 include시켜주어야 한다.
	//위에 #include <string.h> 추가

	char str[] = "Hello";

	int size = sizeof(str) / sizeof(char) - 1; //-1은 맨 끝 null문자(\0)제거
	
	printf("%d\n", size);

	
	
	//함수 이용해서 문자열의 길이 구하기

	char str[100] = "Hello";

	int len;

	len = strlen(str);

	printf("함수를 적용한 문자열의 길이는 : %d\n", len);
	

	//strcpy 함수를 이용한 문자열 복사
	char str1[] = "hello";
	char str2[100];

	strcpy_s(str2, str1); //string 복사

	printf("str2의 값: %s\n", str2);
	

	char str[100] = "Hello ";
	strcat_s(str, "World!");
	printf("%s\n", str);
	*/

	//strcmp - 사전적 순서 비교
	//strcmp(a,b)일 때, a가 b보다 사전순에 앞쪽이면 -1, 뒷쪽이면 1, 같으면 0 반환.

	char str1[] = "sample";
	char str2[] = "simple";

	int cmp = strcmp(str1, str2);
	printf("%d\n", cmp);
}