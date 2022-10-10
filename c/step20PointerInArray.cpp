#include <stdio.h>

int main()
{
	/*
	//배열 포인터 : 배열을 가리키는 포인터(포인터 1개)
	//포인터 배열 : 포인터들의 배열(배열안 값들이 포인터들 -> 포인터가 여러개)

	//예제1
	int a = 10;
	int b = 20;
	int c = 30;
	int d = 40;

	int* ptr[4]; //괄호를 지우면 선언가능

	ptr[0] = &a;
	ptr[1] = &c;
	ptr[2] = &d;
	ptr[3] = &b;

	printf("%d %d %d %d\n", ptr[0], ptr[1], ptr[2], ptr[3]); //차이가 12고 역순이네? 왜지?
	printf("%d %d %d %d\n", *ptr[0], *ptr[1], *ptr[2], *ptr[3]);
	*/

	//예제2

	/*
	//기본지식
	char str[] = "Hello"; //마지막 null값까지 6칸

	printf("%s\n", &str[0]); //%s는 string의 첫번째 주솟값 주면, null값 나올 때까지 출력
	printf("%s\n", str);
	*/
	char strings[3][10] = { "Hello", "World","Doodle" };
	char* p_str[3];

	for (int j = 0; j < 3; j++)
	{
		p_str[j] = strings[j];
	}

	for (int i = 0; i < 3; i++)
	{
		//printf("%s\n", &strings[i][0]);
		printf("%s\n", p_str[i]);
	}
}