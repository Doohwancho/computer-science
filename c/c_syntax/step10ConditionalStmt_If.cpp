#include <stdio.h>

int main()
{
	/*
	int n;
	scanf_s("%d", &n);

	if (n % 2 == 1)
	{
		printf("n은 홀수\n");
	}
	else if(n == 0)
	{
		printf("n은 0\n");
	}
	else
	{
		printf("n은 짝수\n");
	}
	
	//만약 if(1)이면 어떻게 될까?
	//그대로 실행됨. if()는 괄호 안의 숫자가 0이 아니면 ()안의 것을 실행.
	//따라서 if(917)도 실행됨.
	
	if (978)
	{
		printf("if문 test");
	}
	*/

	//중첩 if문
	//중괄호 코딩 스타일

	int a, b, c;

	scanf_s("%d%d%d", &a, &b, &c);

	if (a > b)
	{
		// 1. a > b > c
		// 2. a > c > b
		// 3. c > a > b
		
		if (a > c)  //2번의 경우
			printf("%d\n", a);
		else
			printf("%d\n", c);
	}
	else if (b > c)// b>a   //한줄(세미콜론이 하나면) 중괄호 생략 가능
		printf("%d\n", b);
	else // c > b
		printf("%d\n", c);
}
