#include <stdio.h>

int main()
{
	/*
	int n;
	scanf_s("%d", &n);

	if (n % 2 == 1)
	{
		printf("n�� Ȧ��\n");
	}
	else if(n == 0)
	{
		printf("n�� 0\n");
	}
	else
	{
		printf("n�� ¦��\n");
	}
	
	//���� if(1)�̸� ��� �ɱ�?
	//�״�� �����. if()�� ��ȣ ���� ���ڰ� 0�� �ƴϸ� ()���� ���� ����.
	//���� if(917)�� �����.
	
	if (978)
	{
		printf("if�� test");
	}
	*/

	//��ø if��
	//�߰�ȣ �ڵ� ��Ÿ��

	int a, b, c;

	scanf_s("%d%d%d", &a, &b, &c);

	if (a > b)
	{
		// 1. a > b > c
		// 2. a > c > b
		// 3. c > a > b
		
		if (a > c)  //2���� ���
			printf("%d\n", a);
		else
			printf("%d\n", c);
	}
	else if (b > c)// b>a   //����(�����ݷ��� �ϳ���) �߰�ȣ ���� ����
		printf("%d\n", b);
	else // c > b
		printf("%d\n", c);
}
