#include <stdio.h>

int main()
{
	/*
	int i = 1;
	while (i <= 10)
	{
		printf("%d\n", i);
		i++;
	}
	
	for (int i = 1; i <= 10; i++)
	{
		printf("%d\n", i);
	}

	//일정하게 증가/감소되는 변수가 필요할 때
	//배열의 모든 원소에 순차적으로 접근하고 싶을 때
	//특정 횟수만큼 작업을 반복하고 싶을 때
	

	//예제1. 1,2,4,8,16,32,...

	int n;
	scanf_s("%d", &n);

	for (int i = 1; i <= n; i*=2) //여기에서 정의된 i는 for문 밖에서는 쓸 수 없다.
	{
		printf("%d\n", i);
	}
	
	//예제2 - 1부터 n까지 숫자의 합을 출력

	int n;
	scanf_s("%d", &n);

	int sum = 0;
	for (int i = 1; i <= n; i++)
	{
		sum += i;
	}
	printf("%d\n", sum);
	
	//예제3 - 융통성있는 for문
	int i;

	scanf_s("%d", &i);

	for ( ; ; ) //가운데를 비우면 true라고 간주
	{
		printf("%d", i);
		i++;
	}
	
	//예제4 - 융통성 있는 for문
	int n;
	scanf_s("%d", &n);
	
	int i, sum;
	for (i = 1, sum = 0; i <= n; sum += i, i++);

	printf("%d\n", sum);
	
	
	//예제5 - break: 반복문 한개를 빠져나옴
	for (int i = 1; true; i++)
	{
		printf("%d\n", i);
		if (i == 10) break;
	}
	
	//예제6 - continue : 아래 코드 skip

	int n;
	scanf_s("%d", &n);

	int sum = 0;
	for (int i = 1; i <= n; i++)
	{
		if (i % 2 == 1) continue;
		sum += i;
	}
	printf("%d\n", sum);
	*/
	//예제7 - 중첩 for문

	int n;
	scanf_s("%d", &n);

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			printf("*");
		}
		printf("\n");
	}
}