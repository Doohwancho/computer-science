#include <stdio.h>

int main()
{
	//���������: �������� ����
	//+,-,*,/,=
	// +=.-=, *=, /=, %=
	// ++, --

	//��ġ����� ��ġ����
	/*
	int a = 1;

	printf("%d\n", ++a); //2 -- print�� +1
	printf("%d\n", a++); //2 -- print�� +1
	printf("%d\n", a++); //3 -- print�� +1
	printf("%d", a);     //4
	

	//�� ������, �� ������
	int a, b;
	scanf_s("%d%d", &a, &b);

	//bool: �������� ����� ����(1byte == 8bit)
	//��:1 ����:0
	bool p = a > b;
	bool q = a < b;
	bool r = a == b; // == : ����.

	// a >= b : a�� b���� ũ�ų� ����
	// a <= b : a�� b���� �۰ų� ����
	// a != b : a�� b�� �ƴϴ�.

	printf("%d\n", p);
	printf("%d\n", q);
	printf("%d\n", r);

	*/
	//�� ������(and, or, ...)
	// && || !

	int a;
	scanf_s("%d", &a);

	bool p = a >= 1 && a <= 10; 
	bool q = a == 3 || a == 7;
	bool r = !q;

	printf("%d\n", p);
	printf("%d\n", q);
	printf("%d\n", r);
}