#include <stdio.h>

int main()
{
	float a, b;

	scanf_s("%f%f", &a, &b); 
	//scanf는 보안문제로 2017부터는 scanf_s로 써야함
	//&:포인터. scanf사용하면 포인터 써야한다.
	//scanf의 동작 원리
	//16 7있으면, 16과 7을 각각 숫자%d로 따로 인식을 함. 스페이스가 100번있어도 상관X

	float plus = a + b;
	float minus = a - b;
	float muliply = a * b;
	float divide = a / b; //몫
	//float rest = a % b; //나머지

	printf("%f + %f = %f\n", a, b, plus);
	printf("%f - %f = %f\n", a, b, minus);
	printf("%f * %f = %f\n", a, b, muliply);
	printf("%f / %f = %f\n", a, b, divide);
	//printf("%f %% %f = %f\n", a, b, rest);

}