#include <stdio.h>

int main()
{
	float a, b;

	scanf_s("%f%f", &a, &b); 
	//scanf�� ���ȹ����� 2017���ʹ� scanf_s�� �����
	//&:������. scanf����ϸ� ������ ����Ѵ�.
	//scanf�� ���� ����
	//16 7������, 16�� 7�� ���� ����%d�� ���� �ν��� ��. �����̽��� 100���־ ���X

	float plus = a + b;
	float minus = a - b;
	float muliply = a * b;
	float divide = a / b; //��
	//float rest = a % b; //������

	printf("%f + %f = %f\n", a, b, plus);
	printf("%f - %f = %f\n", a, b, minus);
	printf("%f * %f = %f\n", a, b, muliply);
	printf("%f / %f = %f\n", a, b, divide);
	//printf("%f %% %f = %f\n", a, b, rest);

}