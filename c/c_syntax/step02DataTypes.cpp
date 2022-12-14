#include <stdio.h>

int main()
{
	//%d는 정수 출력(%d == Integer) ex)1,2,3,0,-1,-2,-3
	printf("%d + %d = %d\n", 2, 3, 5); //2 + 3 = 5

	//%f는 실수 출력(%f == float) ex)3.14
	printf("%f\n", 3.14); //3.140000

	//.2는 소숫점 2째자리라는 뜻(3쨋자리에서 반올림)
	printf("%.2f\n", 3.141592); //3.14

	// %g는 실수 출력(지수형태로도 출력) -- 과학 프로그램에서 유용하게 쓰임
	printf("%g\n", 3.141592); //3.14159
	printf("%.3g\n", 87239824.123124); //8.72e+07

	//%c는 문자(char)출력(%c == char) ex)알파벳, 숫자, 몇몇 기호, \n
	printf("%c %c %c\n", 97, 'b', 'c'); //a b c

	//%s는 문자열 출력(문자들의 열거) (%s == String) - 한글 가능
	printf("%s\n", "안녕하세요"); //안녕하세요
}
