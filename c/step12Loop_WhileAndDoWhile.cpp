#include <stdio.h>

int main()
{
	/*
	//�ݺ���(while)
	int i = 1;

	while (i <= 10)
	{
		printf("%d\n", i);
		i++;
	}

	//do-while
	//������ �����ϵ� ���ϵ� ó�� �ѹ� ������ ����
	int i = 15;

	do
	{
		printf("%d\n", i);
		i++;
	} while (i <= 10);
	*/

	//do-while ���� 
	int n;

	do {
		printf("���� 0�� �Է����ּ���!!\n");
		scanf_s("%d", &n);
	} while (n != 0);

	printf("���� 0�� �Է��ϼ̱���!!\n\n^^7");
}