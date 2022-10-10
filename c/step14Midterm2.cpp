//연산자: + - / * % += -= ++ -- && || !
//조건문: if() switch()-case
//반복문: while() do-while() goto for()

#include <stdio.h>

int main()
{
	/*
	//1. 시험점수 입력
	//A: 90~100 
	//B: 80~90
	//C: 70~80
	//D: 60~70
	//E: 50~60

	int score;

	retype:
	scanf_s("%d", &score);

	if (90 <= score && score <= 100)
	{
		printf("A");
	}
	else if (80 <= score && score <= 89)
	{
		printf("B");
	}
	else if (70 <= score && score <= 79)
	{
		printf("C");
	}
	else if (60 <= score && score <= 69)
	{
		printf("D");
	}
	else if (50 <= score && score <= 59)
	{
		printf("E");
	}
	else if (score <= 49)
	{
		printf("F");
	}
	else
	{
		printf("잘못 입력하셨습니다. 다시입력해주세요.\n");
		goto retype;
	}
	*/
	//2. 자연수를 입력받으면 그 자연수의 약수를 모두 출력하라.

	int n;

	scanf_s("%d", &n);

	for (int i = 1; i <= n; i++)
	{
		if (n % i == 0)
		{
			printf("%d, ", i);
		}
	}
}