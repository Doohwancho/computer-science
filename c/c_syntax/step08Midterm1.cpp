#include <stdio.h>

int main()
{
	/*
	//1. �� ���ڸ� �Է� �޾Ƽ� �� ���ڵ��� ���� ����ϴ� ���α׷��� ����� ������.
	int a, b;
	scanf_s("%d%d", &a, &b);
	printf("�� ������ ���� %d", a + b);
	

	//2. ü�߰� Ű�� �Է¹޾Ƽ� ü���� ������ ���ϴ� ���α׷��� ����� ������.
	//BMI = ü��/(Ű)**2
	//����:18.5 ~ 22.9

	float weight, height;
	printf("ü���Է�(kg) : ");
	scanf_s("%f", &weight);
	printf("Ű �Է�(m) : ");
	scanf_s("%f", &height);

	printf("BMI : %.2f", weight / (height * height));
	*/

	//3. ���ĺ��� �Է¹޾Ƽ� �� ���� ���ĺ��� ����ϴ� ���α׷��� ����� ������.(Z����)

	int alphabet;

	printf("�Է��ϰ� ���� ���ĺ���? : ");
	scanf_s("%c", &alphabet); //pointer���� for scanf
	alphabet++;
	printf("�Է��� ���ĺ��� ���� ���ĺ��� %c �Դϴ�.", alphabet);
}


