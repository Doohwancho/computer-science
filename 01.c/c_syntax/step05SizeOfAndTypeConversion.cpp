#include <stdio.h>

int main()
{
	/*
	//A. sizeof(x) : x의 크기를 알려줌
	printf("%d %d %d %d\n", sizeof(int), sizeof(char), sizeof(float), sizeof(double));

	int a; char b; float c; double d;

	printf("%d %d %d %d\n", sizeof(a), sizeof(b), sizeof(c), sizeof(d));
	
	int a = 3.14;
	double b = 10;

	printf("%d %f\n", a, b); // 3, 10.000000

	//실수를 정수형 변수에 담을 수 없다.
	//정수를 실수형 변수에 담을 수 없다.
	*/


	//B. 형변환 : 자료형을 다른 자료형으로 바꾸는 작업
	int math = 90, korean = 95, english = 96;
	int sum = math + korean + english; //solution1: int -> double
	double avg = (double)sum / 3; //solution2: 형변환

	printf("%f\n", avg); 
	//예상 : 93.6667  실제값 : 93.00000 
	//왜? '/'연산자는 몫을 구함
	//정수 / 정수 = 정수 (두개 나눈것 중에서 더 큰것의 자료형을 택함. 따라서 93처럼 float/double이 나와야 하는데 integer가 나옴)
	//실수 / 정수 = 실수
	//실수 / 실수 = 실수

	//정수 + 정수 = 정수
	//정수 + 실수 = 실수
	//실수 + 실수 = 실수
}
