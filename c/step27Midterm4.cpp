#include <stdio.h>

/*
//1. ����������

int useCnt[5] = { 0 };

void useItem(int);

int main()
{
	useItem(4);
	useItem(2);
	useItem(1);
	useItem(4);
	useItem(0);

	for (int i = 0; i < 5; i++)
	{
		printf("����%d�� �������� %d�� ����ϴ�.\n", i, useCnt[i]);
	}
}

void useItem(int itemNum)
{
	useCnt[itemNum]++;
}


//2. ������ ����
//5432112345

//����: �ι�° printf()�� ������� �ʰ� rec(n-1)�� n==0�� �ɸ������� �����ϴٰ�, ������ ���ĺ��� ����

void rec(int n)
{
	if (n == 0) return;
	printf("%d", n); //���� ó��rec()�� print�� ���������� ������ �� �������� ����
	rec(n - 1);
	printf("%d",n);
}

int main()
{
	rec(5);
}
*/
//3. ���ڿ��� �Ű������� �޾� �� ���ڿ����� ������ �����Ͽ� ���

void print_noSpace(char *str) //str[] //���ڿ��� �� �������� null��������
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] != ' ')
			printf("%c", str[i]);
	}
	//for (char* it = str; *it; ++it)
	//{
	//	if(*it != ' ')
	//		printf("%c", *it);
	//}
}

int main()
{
	char str[] = "hello, world!\n";
	print_noSpace(str);
	//print_noSpace("my name is cho\n");
}

