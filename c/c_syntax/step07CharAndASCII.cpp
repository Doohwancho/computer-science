#include <stdio.h>

int main()
{
	/*
	char: 1����Ʈ ������(int:4����Ʈ)
	Character: ���� (�ݰ�����(halfwidth)�� ����. ex)ABCabc1234_+;/#%) �ѱ�,����,���þƾ���� ��������(fullwidth)�� �ȵ�
	*/
	char a = 67;
	
	printf("%d\n", a); //67
	printf("%c\n", a); //C
	printf("%c\n", 'C'); //C
	printf("%d\n", 'C'); //67

	//ASCII : American Standard Code for Information Interchange
	//���ڿ� ���ڸ� ���������ִ� ��

	char b;

	printf("���� �Է� : ");
	scanf_s("%c", &b); //R�� �Է��ϸ�, &b�� 82�� �����
	printf("%c�� ASCII�� : %d\n", b, b); //b�� 82�� ����� ���¿��� %c�ΰ��� R���
}