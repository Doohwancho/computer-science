//������: + - / * % += -= ++ -- && || !
//���ǹ�: if() switch()-case
//�ݺ���: while() do-while() goto for()

#include <stdio.h>

int main()
{
	/*
	//1. �������� �Է�
	//A: 90~100 
	//B: 80~90
	//C: 70~80
	//D: 60~70
	//E: 50~60

	int score;

	retype:
	scanf_s("%d", &score);

	if (90 <= score && score <= 100)
	{
		printf("A");
	}
	else if (80 <= score && score <= 89)
	{
		printf("B");
	}
	else if (70 <= score && score <= 79)
	{
		printf("C");
	}
	else if (60 <= score && score <= 69)
	{
		printf("D");
	}
	else if (50 <= score && score <= 59)
	{
		printf("E");
	}
	else if (score <= 49)
	{
		printf("F");
	}
	else
	{
		printf("�߸� �Է��ϼ̽��ϴ�. �ٽ��Է����ּ���.\n");
		goto retype;
	}
	*/
	//2. �ڿ����� �Է¹����� �� �ڿ����� ����� ��� ����϶�.

	int n;

	scanf_s("%d", &n);

	for (int i = 1; i <= n; i++)
	{
		if (n % i == 0)
		{
			printf("%d, ", i);
		}
	}
}