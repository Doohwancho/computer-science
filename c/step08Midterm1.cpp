#include <stdio.h>

int main()
{
	/*
	//1. 두 숫자를 입력 받아서 그 숫자들의 합을 출력하는 프로그램을 만들어 보세요.
	int a, b;
	scanf_s("%d%d", &a, &b);
	printf("두 숫자의 합은 %d", a + b);
	

	//2. 체중과 키를 입력받아서 체질량 지수를 구하는 프로그램을 만들어 보세요.
	//BMI = 체중/(키)**2
	//정상:18.5 ~ 22.9

	float weight, height;
	printf("체중입력(kg) : ");
	scanf_s("%f", &weight);
	printf("키 입력(m) : ");
	scanf_s("%f", &height);

	printf("BMI : %.2f", weight / (height * height));
	*/

	//3. 알파벳을 입력받아서 그 다음 알파벳을 출력하는 프로그램을 만들어 보세요.(Z제외)

	int alphabet;

	printf("입력하고 싶은 알파벳은? : ");
	scanf_s("%c", &alphabet); //pointer선언 for scanf
	alphabet++;
	printf("입력한 알파벳의 다음 알파벳은 %c 입니다.", alphabet);
}


