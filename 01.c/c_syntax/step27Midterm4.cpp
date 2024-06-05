#include <stdio.h>

/*
//1. 실행결과예측

int useCnt[5] = { 0 };

void useItem(int);

int main()
{
	useItem(4);
	useItem(2);
	useItem(1);
	useItem(4);
	useItem(0);

	for (int i = 0; i < 5; i++)
	{
		printf("슬롯%d의 아이템을 %d번 썼습니다.\n", i, useCnt[i]);
	}
}

void useItem(int itemNum)
{
	useCnt[itemNum]++;
}


//2. 실행결과 예측
//5432112345

//절차: 두번째 printf()는 실행되지 않고 rec(n-1)이 n==0에 걸릴때까지 실행하다가, 끝나면 이후부터 실행

void rec(int n)
{
	if (n == 0) return;
	printf("%d", n); //가장 처음rec()은 print는 먼저되지만 끝날땐 맨 마지막에 끝남
	rec(n - 1);
	printf("%d",n);
}

int main()
{
	rec(5);
}
*/
//3. 문자열을 매개변수로 받아 그 문자열에서 공백을 제거하여 출력

void print_noSpace(char *str) //str[] //문자열도 맨 마지막에 null문자있음
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] != ' ')
			printf("%c", str[i]);
	}
	//for (char* it = str; *it; ++it)
	//{
	//	if(*it != ' ')
	//		printf("%c", *it);
	//}
}

int main()
{
	char str[] = "hello, world!\n";
	print_noSpace(str);
	//print_noSpace("my name is cho\n");
}

