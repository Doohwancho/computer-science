#include <stdio.h>

int main()
{
	/*
	//A. sizeof(x) : x�� ũ�⸦ �˷���
	printf("%d %d %d %d\n", sizeof(int), sizeof(char), sizeof(float), sizeof(double));

	int a; char b; float c; double d;

	printf("%d %d %d %d\n", sizeof(a), sizeof(b), sizeof(c), sizeof(d));
	
	int a = 3.14;
	double b = 10;

	printf("%d %f\n", a, b); // 3, 10.000000

	//�Ǽ��� ������ ������ ���� �� ����.
	//������ �Ǽ��� ������ ���� �� ����.
	*/


	//B. ����ȯ : �ڷ����� �ٸ� �ڷ������� �ٲٴ� �۾�
	int math = 90, korean = 95, english = 96;
	int sum = math + korean + english; //solution1: int -> double
	double avg = (double)sum / 3; //solution2: ����ȯ

	printf("%f\n", avg); 
	//���� : 93.6667  ������ : 93.00000 
	//��? '/'�����ڴ� ���� ����
	//���� / ���� = ���� (�ΰ� ������ �߿��� �� ū���� �ڷ����� ����. ���� 93ó�� float/double�� ���;� �ϴµ� integer�� ����)
	//�Ǽ� / ���� = �Ǽ�
	//�Ǽ� / �Ǽ� = �Ǽ�

	//���� + ���� = ����
	//���� + �Ǽ� = �Ǽ�
	//�Ǽ� + �Ǽ� = �Ǽ�
}
