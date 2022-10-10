#include <stdio.h>

//call-by-reference
//arr[4] = *arr
//arr = &arr[0]

//void printArr(int arr[2][3]) 
void printArr(int(*arr)[3]) //��ȣ�� ������ �迭������. ������ �����͵��� ����ִ� �迭
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			arr[i][j] *= 2;
		}
	}
}

int main()
{
	int arr[2][3] = { { 1,2,3},{4,5,6} };
	printArr(arr);

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%d", arr[i][j]);
		}
		printf("\n");
	}
}