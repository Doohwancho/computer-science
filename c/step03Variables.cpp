#include <stdio.h>
//step03 - �ڷ���


//���� ������ ������ �� �ִ� ������ �����ϰ� ����(������)�� �ҷ��´�.

int main()
{
	/*
	//����
	int a; //int�� 32bit��ŭ�� ������ �Ҵ��ϰڴٶ�� ��. 8bit == 1byte�ϱ� 32bit == 4byte

	//���Ԥ�
	a = 4;
	printf("%d\n", a);

	//�ʱ�ȭ(����+����)
	int b = 3;

	//���� �̸�: ���ĺ� ��ҹ���, _, ����(ù��° ����)
	int plus = a + b;
	int minus = a - b;
	int muliply = a * b;
	int divide = a / b; //��
	int rest = a % b; //������

	printf("%d + %d = %d\n", a, b, plus);
	printf("%d - %d = %d\n", a, b, minus);
	printf("%d * %d = %d\n", a, b, muliply);
	printf("%d / %d = %d\n", a, b, divide);
	printf("%d %% %d = %d\n", a, b, rest); //%�� Ȧ�ξ��� �ȵǰ� %%��� ǥ���ؾ� %��� ����
	*/
	/*
	�ȳ� ���� ��Ը� �ּ��̾�
	*/
	
	//Ctrl+h �� �ϰ����� ����
	//float�� 32bit(4byte), �Ǽ��� ��µ� ���� 
	//�۵� ���� : 3.14 = [+,-2,314] 
	//�̰� �����ϱ� ���ؼ� ū�뷮�� �ʿ��ѵ� �־��� �뷮�� �۾Ƽ� �����߻� -> ������ ��� double���̾�
	//float = �ε��Ҽ��� = �ε��� ���ٴѴٴ� �� = floating point = �Ҽ����� ���ٴϴϱ�.
	/*
	float a = 9.8;
	float b = 3.14;

	float plus = a + b;
	float minus = a - b;
	float muliply = a * b;
	float divide = a / b; //��
	
	
	printf("%.2f + %.2f = %.2f\n", a, b, plus);
	printf("%.2f - %.2f = %.2f\n", a, b, minus);
	printf("%.2f * %.2f = %.2f\n", a, b, muliply);
	printf("%.2f / %.2f = %.2f\n", a, b, divide);
	*/

	//double - 64bit(8byte), �Ǽ��� ��µ� ����. float���� �뷮�� 2��� ��Ȯ���� ���
	double a = 9.8;
	double b = 3.14;

	double plus = a + b;
	double minus = a - b;
	double muliply = a * b;
	double divide = a / b; //��


	printf("%.2f + %.2f = %.2f\n", a, b, plus);
	printf("%.2f - %.2f = %.2f\n", a, b, minus);
	printf("%.2f * %.2f = %.2f\n", a, b, muliply);
	printf("%.2f / %.2f = %.2f\n", a, b, divide);

}