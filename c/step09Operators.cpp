#include <stdio.h>

int main()
{
	//산술연산자: 수학적인 연산
	//+,-,*,/,=
	// +=.-=, *=, /=, %=
	// ++, --

	//전치연산과 후치연산
	/*
	int a = 1;

	printf("%d\n", ++a); //2 -- print전 +1
	printf("%d\n", a++); //2 -- print후 +1
	printf("%d\n", a++); //3 -- print후 +1
	printf("%d", a);     //4
	

	//비교 연산자, 논리 연산자
	int a, b;
	scanf_s("%d%d", &a, &b);

	//bool: 진리값을 저장용 변수(1byte == 8bit)
	//참:1 거짓:0
	bool p = a > b;
	bool q = a < b;
	bool r = a == b; // == : 같다.

	// a >= b : a는 b보다 크거나 같다
	// a <= b : a는 b보다 작거나 같다
	// a != b : a는 b가 아니다.

	printf("%d\n", p);
	printf("%d\n", q);
	printf("%d\n", r);

	*/
	//논리 연산자(and, or, ...)
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