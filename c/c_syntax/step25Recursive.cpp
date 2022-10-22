#include <stdio.h>

//재귀 함수(recursion)
//자기 정의 안에 자기 자신이 등장하는 것
//실행순서는 가장 안쪽에서 역순으로.
//void rec(int n)
//{
//	if (n > 5) return;
//	printf("n = %d\n", n);
//	rec(n+1);
//}

//팩토리얼
//5! = 1*2*3*4*5 = 120
//n! = n* (n-1)!

int factorial(int n)
{
	if (n == 1) return 1;
	return n * factorial(n - 1);
}

int main() {
	//rec(1);
	int rst = factorial(5);
	printf("%d\n", rst);
}