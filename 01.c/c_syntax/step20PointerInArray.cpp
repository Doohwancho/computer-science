#include <stdio.h>

int main()
{
	/*
	//�迭 ������ : �迭�� ����Ű�� ������(������ 1��)
	//������ �迭 : �����͵��� �迭(�迭�� ������ �����͵� -> �����Ͱ� ������)

	//����1
	int a = 10;
	int b = 20;
	int c = 30;
	int d = 40;

	int* ptr[4]; //��ȣ�� ����� ���𰡴�

	ptr[0] = &a;
	ptr[1] = &c;
	ptr[2] = &d;
	ptr[3] = &b;

	printf("%d %d %d %d\n", ptr[0], ptr[1], ptr[2], ptr[3]); //���̰� 12�� �����̳�? ����?
	printf("%d %d %d %d\n", *ptr[0], *ptr[1], *ptr[2], *ptr[3]);
	*/

	//����2

	/*
	//�⺻����
	char str[] = "Hello"; //������ null������ 6ĭ

	printf("%s\n", &str[0]); //%s�� string�� ù��° �ּڰ� �ָ�, null�� ���� ������ ���
	printf("%s\n", str);
	*/
	char strings[3][10] = { "Hello", "World","Doodle" };
	char* p_str[3];

	for (int j = 0; j < 3; j++)
	{
		p_str[j] = strings[j];
	}

	for (int i = 0; i < 3; i++)
	{
		//printf("%s\n", &strings[i][0]);
		printf("%s\n", p_str[i]);
	}
}