#include <stdio.h>

//��� �Լ�(recursion)
//�ڱ� ���� �ȿ� �ڱ� �ڽ��� �����ϴ� ��
//��������� ���� ���ʿ��� ��������.
//void rec(int n)
//{
//	if (n > 5) return;
//	printf("n = %d\n", n);
//	rec(n+1);
//}

//���丮��
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