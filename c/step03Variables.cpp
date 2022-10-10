#include <stdio.h>
//step03 - 자료형


//램에 변수를 선언할 수 있는 공간을 마련하고 변수(데이터)를 불러온다.

int main()
{
	/*
	//선언
	int a; //int는 32bit만큼을 램에다 할당하겠다라는 뜻. 8bit == 1byte니까 32bit == 4byte

	//대입ㅣ
	a = 4;
	printf("%d\n", a);

	//초기화(선언+대입)
	int b = 3;

	//변수 이름: 알파벳 대소문자, _, 숫자(첫번째 글자)
	int plus = a + b;
	int minus = a - b;
	int muliply = a * b;
	int divide = a / b; //몫
	int rest = a % b; //나머지

	printf("%d + %d = %d\n", a, b, plus);
	printf("%d - %d = %d\n", a, b, minus);
	printf("%d * %d = %d\n", a, b, muliply);
	printf("%d / %d = %d\n", a, b, divide);
	printf("%d %% %d = %d\n", a, b, rest); //%는 홀로쓰면 안되고 %%라고 표기해야 %라고 나옴
	*/
	/*
	안녕 나는 대규모 주석이야
	*/
	
	//Ctrl+h 로 일괄변경 가능
	//float는 32bit(4byte), 실수를 담는데 쓰임 
	//작동 원리 : 3.14 = [+,-2,314] 
	//이걸 저장하기 위해서 큰용량이 필요한데 주어진 용량은 작아서 오차발생 -> 요즘은 대신 double많이씀
	//float = 부동소숫점 = 부동은 떠다닌다는 뜻 = floating point = 소숫점이 떠다니니깐.
	/*
	float a = 9.8;
	float b = 3.14;

	float plus = a + b;
	float minus = a - b;
	float muliply = a * b;
	float divide = a / b; //몫
	
	
	printf("%.2f + %.2f = %.2f\n", a, b, plus);
	printf("%.2f - %.2f = %.2f\n", a, b, minus);
	printf("%.2f * %.2f = %.2f\n", a, b, muliply);
	printf("%.2f / %.2f = %.2f\n", a, b, divide);
	*/

	//double - 64bit(8byte), 실수를 담는데 쓰임. float보다 용량이 2배라 정확도도 상승
	double a = 9.8;
	double b = 3.14;

	double plus = a + b;
	double minus = a - b;
	double muliply = a * b;
	double divide = a / b; //몫


	printf("%.2f + %.2f = %.2f\n", a, b, plus);
	printf("%.2f - %.2f = %.2f\n", a, b, minus);
	printf("%.2f * %.2f = %.2f\n", a, b, muliply);
	printf("%.2f / %.2f = %.2f\n", a, b, divide);

}