#include <stdio.h>

//��������
int itemCnt = 0;
int money = 100;

void buyItem(int cost)
{
	//�������� ���.
	itemCnt++;
	money -= cost;
	printf("�������� �����߽��ϴ�.\n");
	printf(" ������ ���� : %d\n", itemCnt);
	printf(" �ܾ� : %d\n", money);
}

int plus(int a, int b) //int type ��ȯ
{
	return a + b;
}

int noMeaning()
{
	printf("first\n");
	return 1;
	printf("second\n"); //������ ������ ��� �Լ� �����ϱ� ������ 2�� ����ȵ�.
	return 2;
}

void noMeaningtwo()
{
	printf("noMeaning\n");
	return; //void�� �׳� return;
}

int main()
{
	buyItem(30);

	buyItem(50);

	int sum = plus(3, 5);
	printf("%d\n", sum);

	int a;
	a = noMeaning();
	printf("��ȯ�� �� : %d\n", a);

	noMeaningtwo();

	return 0; //������� �ʾƵ� main�� return 0�� ��.
	//��? 0 �̿��� ���� �����ڵ��̱� ����. ���� ��ȯ�Ǵ� ���� 0�� TRUE�̱� ����.
	//�� ����? ������ ������ n�� ��ȯ�ϰ� ���� �� 0�� ��ȯ�ϸ�, 
	//if(main()) { ����ó�� } �� 0�� ��ȯ�ؾ� �����̸� ==������ ������� �ʾƵ� ��.  
}