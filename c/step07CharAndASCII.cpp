#include <stdio.h>

int main()
{
	/*
	char: 1바이트 정수형(int:4바이트)
	Character: 문자 (반각문자(halfwidth)만 가능. ex)ABCabc1234_+;/#%) 한글,한자,러시아어등은 정각문자(fullwidth)라서 안됨
	*/
	char a = 67;
	
	printf("%d\n", a); //67
	printf("%c\n", a); //C
	printf("%c\n", 'C'); //C
	printf("%d\n", 'C'); //67

	//ASCII : American Standard Code for Information Interchange
	//문자와 숫자를 대응시켜주는 것

	char b;

	printf("문자 입력 : ");
	scanf_s("%c", &b); //R을 입력하면, &b에 82가 저장됨
	printf("%c의 ASCII값 : %d\n", b, b); //b가 82가 저장된 상태에서 %c로가면 R출력
}