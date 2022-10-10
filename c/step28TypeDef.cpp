#include <stdio.h>

//type define = 형을 정의


int main()
{
	//unsigned short int age;

	typedef int int32;
	int32 n = 20; //int가 몇비트인지 알려주기 위해 int32로바꿈

	printf("%d\n", n);
	printf("%d\n", sizeof(int32)); //4 == int와 같음

	typedef int Pair[2]; //2개짜리 int형 배열 생성
	Pair point = { 3,4 }; // int point[2] = {3,4};
	//점의 좌표라고 가정

	printf("(%d, %d)\n", point[0], point[1]);

	/*
	<문제>

	typedef char *String;

	String name = "Doohwan";

	시 에러코드
	error C2440:  '초기화 중': 'const char [8]'에서 'String'(으)로 변환할 수 없습니다.


	<원인>

	"C에서는 문자열 리터럴이 char의 배열이지만, C++에서는 const char의 배열이다."
	즉, char *에 문자열 리터럴을 담는 방식은 C 스타일이며, C++에서는 잘못된 코딩방법이기 때문에 C2440 오류가 발생하는 것이다.


	<해결방안>

	1. pointer가 아닌 array 형식으로 선언

	char s1[] = "aa";


	2. (char *) 형식으로 형변환(casting)

	char *s1 = (char*)"aa";


	3. 자료형을 const char* 형태로 바꾸기

	const char *s1 = "aa";


	4. const_cast<char*> 사용하기

	char *s1 = const_cast<char*>("aa");
	*/

	typedef char String;

	String name1[] = "Doohwan"; // char *name = "Doohwan";

	printf("이름: %s\n", name1);

	typedef const char *Stringtwo;

	Stringtwo name2 = "Daehwan"; // char *name = "Doohwan";

	printf("이름: %s\n", name2);

}