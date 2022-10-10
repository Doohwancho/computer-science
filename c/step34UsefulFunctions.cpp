#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	/*
	//������ �Լ���
	//scanf�� printf�� �����ϴ� �Լ�

	//����1 - getchar / putchar
	char ch;

	ch = getchar();
	putchar(ch);

	//����2 - gets / puts
	char str[100];

	gets_s(str); //�迭��ü str�� call by reference�� �ȴ�
	puts(str);
	
	
	//����3 - sscanf / sprintf
	//���ڿ� ��ü���ٰ� printf, scanf�ϴ� ��

	char str[] = "450";
	int n;

	sscanf_s(str, "%d", &n);
	printf("n�� ��: %d\n", n);
	

	//����4
	int n = 450;
	char str[100];

	sprintf_s(str,"%d", n);
	printf("%s\n", str);
	

	//����5 - ����
	//#include <stdlib.h> �߰�

	//�õ尪�� ����ð����� �־����
	srand(time(NULL)); // 1970/01/01 00:00:00 ���� ���� �ð�, �ʴ��� ���

	for (int i = 1; i <= 10; i++)
	{
		printf("%d\n", rand() %10 + 1); //���� ���� 
	}
	*/

	//����6 - exit(0);
	//return 0�� ���, �ٷ� ���α׷� ����

	int sum = 0;

	for (int i = 0; i < 5; i++)
	{
		int n;

		scanf_s("%d", &n);

		if (n <= 0) {
			printf("INPUT ERROR\n");
			exit(0);
		}

		sum += n;
	}
	printf("%d\n", sum);

}