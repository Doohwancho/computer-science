#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	/*
	//유용한 함수들
	//scanf와 printf를 보완하는 함수

	//예제1 - getchar / putchar
	char ch;

	ch = getchar();
	putchar(ch);

	//예제2 - gets / puts
	char str[100];

	gets_s(str); //배열자체 str이 call by reference가 된다
	puts(str);
	
	
	//예제3 - sscanf / sprintf
	//문자열 자체에다가 printf, scanf하는 것

	char str[] = "450";
	int n;

	sscanf_s(str, "%d", &n);
	printf("n의 값: %d\n", n);
	

	//예제4
	int n = 450;
	char str[100];

	sprintf_s(str,"%d", n);
	printf("%s\n", str);
	

	//예제5 - 난수
	//#include <stdlib.h> 추가

	//시드값을 현재시간으로 넣어버림
	srand(time(NULL)); // 1970/01/01 00:00:00 부터 지난 시간, 초단위 출력

	for (int i = 1; i <= 10; i++)
	{
		printf("%d\n", rand() %10 + 1); //같은 숫자 
	}
	*/

	//예제6 - exit(0);
	//return 0과 비슷, 바로 프로그램 종료

	int sum = 0;

	for (int i = 0; i < 5; i++)
	{
		int n;

		scanf_s("%d", &n);

		if (n <= 0) {
			printf("INPUT ERROR\n");
			exit(0);
		}

		sum += n;
	}
	printf("%d\n", sum);

}