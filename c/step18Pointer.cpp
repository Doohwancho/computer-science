/*
	������(pointer)
	������ ����Ű�� �� - ������ �ּҸ� �����ϴ� ����
	
	�������, a=20�� �� ��򰡿� a��� ������ 20�� ������ �ִٰ� ����Ǿ�����.
	�����͵� �� ��򰡿� ����Ǿ� �ִ� ������.
	a�� 20�ΰ� �˷��� a�� �����Ͱ� ã�ƾ� ��.
	��� ã�ĸ� a�� �ּҰ����� ã��
	�� �ȿ��� ������ �������� ����Ǿ� �ֱ� ������, a�� ������ ���� a�� �ڸ��� �ּҰ��� ����
	������� 1125��° �ڸ��̴�.
	�� 1125��° �ڸ��̴ٶ�� ���� �������� �����Ͱ� a�� ã��
*/

#include <stdio.h>

int main() 
{
	/*
	//����1
	int a = 20;
	
	int *ptr_a; //����ų ������ �� + ��ǥ

	ptr_a = &a; //&�� �ּҰ��� ����. ex) &main�� ������ �ּڰ���.

	printf("%d\n", ptr_a); //10157184 , 9436640 , ... 
	//������ ������ �޶���. ��? ������ ������ ���� ��Ȳ�� �޶����� ����

	printf("a�� �� : %d\n", a); 
	printf("a�� �ּڰ� : %d\n", &a); //a�� �ּڰ�
	printf("ptr_a�� ����� �� : %d\n", ptr_a); //�����Ͱ� ����Ű�� ������ �ּڰ�
	printf("ptr_a�� ����Ű�� ������ �� : %d\n", *ptr_a); //*�� ptr_a�� ����Ű�� ���� ��ü�� ����ϰ� ���� �� (==a)
	

	//����2
	int a = 10;
	int b = 20;

	int* ptr;

	ptr = &a;
	printf("ptr�� ����Ű�� ������ ����� �� : %d\n", *ptr);

	ptr = &b;
	printf("ptr�� ����Ű�� ������ ����� �� : %d\n", *ptr);
	

	//����3
	int a = 10;

	int* ptr;
	ptr = &a;

	printf("a�� �� : %d\n", a);

	*ptr = 20; //a�� �ּڰ��� 20�� ����
	
	printf("a�� �� : %d\n", a);
	

	//����4
	int a = 10;

	int* ptr;
	ptr = &a;

	int** ptr_ptr;
	ptr_ptr = &ptr;

	printf("a = %d\n", a); //a�� ��
	printf("&a = %d\n", &a); //a�� �ּڰ�
	
	printf("ptr = %d\n", ptr); //a�� �ּڰ�
	printf("&ptr = %d\n", &ptr); //a�� �ּڰ��� �����ϰ� �ִ� ptr�� �ּڰ�

	printf("ptr_ptr = %d\n", ptr_ptr); //a�� �ּڰ��� �����ϰ� �ִ� ptr�� �ּڰ�
	printf("*ptr_ptr = %d\n", *ptr_ptr); //a�� �ּڰ�
	printf("**ptr_ptr = %d\n", **ptr_ptr); //a�� ��

	
	//����5
	int a = 10;
	int* ptr_a;
	ptr_a = &a;


	printf("ptr_a�� �� : %d\n", ptr_a); //9698224
	printf("ptr_a+1�� ��: %d\n", ptr_a + 1); //9698228 -- �� +1�� �ƴϰ� +4?
	//��ǻ��(��)�� 1����Ʈ�� ���� ���� ó������.(1����Ʈ�� 8��Ʈ. 2������ ���°� 8��)
	//int�� 4����Ʈ. +1�ϸ� +4����Ʈ.
	//�׷��� �����Ϳ� +n�ϸ�, �����Ͱ� ����Ű���ִ� ���� ũ�⸸ŭ ����

	//����6
	int arr[5] = { 1,2,3,4,5 };

	printf("arr�� �� : %d\n", arr); //7337936 == arr[0] -> arr == &arr[0]
	for (int i = 0; i < 5; i++)
	{
		//int�� 4����Ʈ�� ������ �޸𸮿� �����ϴ±���
		printf("&arr[%d] = %d\n", i, &arr[i]); //7337936  7337940  7337944  7337948  7337952

		printf("arr + %d = %d\n", i, arr + i);
	}
	

	//����7
	int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int* ptr = arr;

	for (int i = 0; i < 10; i++)
	{
		printf("%d", arr[i]);
	}
	printf("\n");

	for (int i = 0; i < 10; i++)
	{
		printf("%d", *(arr + i));
	}
	printf("\n");

	for (int* ptr = arr; ptr < arr + 10; ptr++) //���⼭ ptr++�� int���̱⿡ +4�̰�, +10�� 40��
	{
		printf("%d", *ptr);
	}
	printf("\n");

	for (int i = 0; i < 10; i++)
	{
		printf("%d", i[ptr]);
	}
	printf("\n");
	// arr[i] == *(arr+i) == *(ptr+i) == *(i+ptr) == i[ptr]
	//��?
	//��ǻ�ʹ� a[b] == *(a+b)�� �ν��ϱ� ������
	//a+b�� b+a�� �ٲپ� b[a]�� ���� ������� ���´�.
	//���̱�
	*/
	/*
		����

		1. arr == &arr[0] (ptr == &ptr[0])
		2. *ptr == ptr[0]
		3. ptr + 1 == ptr�� siszeof(*ptr)�� ���� ��
	*/
	//����8
	int arr[4] = { 1,2,3,4 };

	printf("arr = %d\n", arr);
	printf("arr + 1 = %d\n", arr + 1); //+4 arr[0]�� sizeof(int)��ŭ 4����

	printf("&arr = %d\n", &arr);
	printf("&arr + 1 = %d\n", &arr + 1); //+12 ��? sizeof(array)��ŭ 4*n����

}