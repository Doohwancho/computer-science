//prototype : ����(���� ���), ����ǰ
//�뵵:�ȱ� �� ���� ���ٰ� ���� �� �ȱ� ���������, C�� �������̶� �������°��, ���� �����س��� ���߿� ���� �����ϴ°�
//���� �Լ����� ���� ������ ���� �Ű澵 �ʿ䰡 ����, �� ���Ͽ� � �Լ��� �ִ��� �Ѵ��� �� �� �ְ� ���ش�.

#include <stdio.h>

//prototype
void walk(int);
void rotate(int);
void drawSquare();

//�ȱ�
void walk(int distance)
{
	printf("%dcm�� �ɾ����ϴ�.\n", distance);
}

//����
void rotate(int angle)
{
	printf("%d�� ȸ���߽��ϴ�.\n", angle);
}

void drawSquare()
{
	for (int i = 1; i <= 4; i++)
	{
		walk(10);
		rotate(90);
	}
}


int main()
{
	drawSquare();
}