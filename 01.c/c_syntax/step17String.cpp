#include <stdio.h>
#include <string.h>

int main()
{
	/*
	//���ڿ� : ���ڵ��� ����
	char arr[] = "abc";
	
	printf("%s\n", arr);

	printf("�迭�� ũ��: %d\n", sizeof(arr) / sizeof(char));
	//�迭 �� ���ҵ��� 3���ε� �迭�� ũ�Ⱑ 4��� ������ ������?
	//����
	//char arr[] = {'a','b','c','\0'}; ���� ����Ǽ� �׷�.
	//\0�� null������. ���Ⱑ �ٷ� ���ڿ��� ���̴� ��� ���� ��Ÿ��
	//���� \0�� �߰��� ������ �� �ڿ� ���ڵ��� ó������ ����
	
	char s[1000];

	scanf_s("%s", s, 1000); 
	//s�տ��� &�� ������ �ʴ´�.
	//���� �迭�� ũ�⸦ ���� �����־�� �Ѵ�
	//��?.
	//�ֳ��ϸ� scanf_s�Լ��� ���� _s�� ���� �Լ����� ���ڿ��� ���� �� 
	//����ڰ� �Է��� ������ ������ �޸��� ũ�⸦ �Ѿ�� ������ �������� 
	//����� �޸��� ũ�⸦ ����ؾ� �ϱ� ����.
	
	printf("%s\n", s);
	

	//���ڿ��� ���õ� 4���� �Լ�
	//stdio.hó�� ���ڿ��� ���õ� �Լ��� ������, ���ο� ������ include�����־�� �Ѵ�.
	//���� #include <string.h> �߰�

	char str[] = "Hello";

	int size = sizeof(str) / sizeof(char) - 1; //-1�� �� �� null����(\0)����
	
	printf("%d\n", size);

	
	
	//�Լ� �̿��ؼ� ���ڿ��� ���� ���ϱ�

	char str[100] = "Hello";

	int len;

	len = strlen(str);

	printf("�Լ��� ������ ���ڿ��� ���̴� : %d\n", len);
	

	//strcpy �Լ��� �̿��� ���ڿ� ����
	char str1[] = "hello";
	char str2[100];

	strcpy_s(str2, str1); //string ����

	printf("str2�� ��: %s\n", str2);
	

	char str[100] = "Hello ";
	strcat_s(str, "World!");
	printf("%s\n", str);
	*/

	//strcmp - ������ ���� ��
	//strcmp(a,b)�� ��, a�� b���� �������� �����̸� -1, �����̸� 1, ������ 0 ��ȯ.

	char str1[] = "sample";
	char str2[] = "simple";

	int cmp = strcmp(str1, str2);
	printf("%d\n", cmp);
}