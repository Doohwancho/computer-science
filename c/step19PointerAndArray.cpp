#include <stdio.h>

int main()
{
	/*
	//����1 - �迭 ������ : �迭�� ����Ű�� ������
	int arr[3] = { 1,2,3 };

	int(*ptr_arr)[3]; //���� 3�� int�� �迭�� ����Ű�� �����͸� ����
	ptr_arr = &arr;

	for (int i = 0; i < 3; i++)
	{
		printf("%d\n", (*ptr_arr)[i]);
	}
	
	//����2 - 2���� �迭�� �迭������
	int arr[2][3] = { {1,2,3},{4,5,6} };

	printf("%d\n", sizeof(arr)); //24
	printf("%d\n", sizeof(arr[0])); //12 
	printf("%d\n", sizeof(arr[0][0])); //4

	printf("%d\n", &arr);  //�� �� ��� ���� ��(arr[0]�� �ּڰ�)
	printf("%d\n", &arr[0]);  
	printf("%d\n", &arr[0][0]);
	
	
	//����3 - 2���� �迭 ������

	int arr[2][3] = { {1,2,3},{4,5,6} };
	
	int(*ptr)[3] = arr;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%d", ptr[i][j]);
			printf("%d",sizeof(ptr[i]));   //12 - arr[0]�� ��ü int�� ������
			printf("%d", sizeof(ptr[i][j])); //4 - int�� ������
		}
		printf("\n");
	}
	*/
	//����4 -- �迭������ ����

	int arr[2][3] = { {1,2,3},{4,5,6} };

	for (int(*row)[3] = arr; row < arr + 2; row++)
	{
		for (int* col = *row; col < *row + 3; col++)
		{
			printf("%d", *col);
		}
		printf("%\n");
	}
}