#include <stdio.h>

int main()
{
	/*
	int i = 1;
	while (i <= 10)
	{
		printf("%d\n", i);
		i++;
	}
	
	for (int i = 1; i <= 10; i++)
	{
		printf("%d\n", i);
	}

	//�����ϰ� ����/���ҵǴ� ������ �ʿ��� ��
	//�迭�� ��� ���ҿ� ���������� �����ϰ� ���� ��
	//Ư�� Ƚ����ŭ �۾��� �ݺ��ϰ� ���� ��
	

	//����1. 1,2,4,8,16,32,...

	int n;
	scanf_s("%d", &n);

	for (int i = 1; i <= n; i*=2) //���⿡�� ���ǵ� i�� for�� �ۿ����� �� �� ����.
	{
		printf("%d\n", i);
	}
	
	//����2 - 1���� n���� ������ ���� ���

	int n;
	scanf_s("%d", &n);

	int sum = 0;
	for (int i = 1; i <= n; i++)
	{
		sum += i;
	}
	printf("%d\n", sum);
	
	//����3 - ���뼺�ִ� for��
	int i;

	scanf_s("%d", &i);

	for ( ; ; ) //����� ���� true��� ����
	{
		printf("%d", i);
		i++;
	}
	
	//����4 - ���뼺 �ִ� for��
	int n;
	scanf_s("%d", &n);
	
	int i, sum;
	for (i = 1, sum = 0; i <= n; sum += i, i++);

	printf("%d\n", sum);
	
	
	//����5 - break: �ݺ��� �Ѱ��� ��������
	for (int i = 1; true; i++)
	{
		printf("%d\n", i);
		if (i == 10) break;
	}
	
	//����6 - continue : �Ʒ� �ڵ� skip

	int n;
	scanf_s("%d", &n);

	int sum = 0;
	for (int i = 1; i <= n; i++)
	{
		if (i % 2 == 1) continue;
		sum += i;
	}
	printf("%d\n", sum);
	*/
	//����7 - ��ø for��

	int n;
	scanf_s("%d", &n);

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			printf("*");
		}
		printf("\n");
	}
}