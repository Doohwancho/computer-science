#include <stdio.h>

int main()
{
	/*
	//반복문(while)
	int i = 1;

	while (i <= 10)
	{
		printf("%d\n", i);
		i++;
	}

	//do-while
	//조건을 만족하든 안하든 처음 한번 무조건 실행
	int i = 15;

	do
	{
		printf("%d\n", i);
		i++;
	} while (i <= 10);
	*/

	//do-while 예제 
	int n;

	do {
		printf("제발 0을 입력해주세요!!\n");
		scanf_s("%d", &n);
	} while (n != 0);

	printf("드디어 0을 입력하셨군요!!\n\n^^7");
}