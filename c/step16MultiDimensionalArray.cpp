#include <stdio.h>

int main()
{
	/*
	//2���� �迭
	int arr[3][4] = { //3�� 4��¥�� 2���� �迭
		{1,2,3,4},
		{5,6,7,8},
		{9,10,11,12}
	};

	//�̷��� ��� �Ǳ� �ȴ�;
	int arr1[3][4] = { 1,2,3,4,5,6,7,8,9,10,11,12 };
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%d\t", arr[i][j]);
		}
		printf("\n");
	}
	*/

	//�Ľ�Į�� �ﰢ��
	/*
	1
	11
	121
	1331
	...
	j==0 || j== i -> 1
	i�� j�� = [i-1 j-1]+[i-1 j]
	*/
	int p[10][10];

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			if (j == 0 || j == i)
			{
				p[i][j] = 1;
			}
			else
			{
				p[i][j] = p[i - 1][j - 1] + p[i - 1][j];
			}
			printf("%d", p[i][j]);
		}
		printf("\n");
	}
	
	
}