#include <stdio.h>

int main()
{
	/*
	//A. List
	int a[3]; //[0,0,0,0,0];

	a[0] = 2; //[2,0,0,0,0];
	a[1] = 100;
	a[2] = 7;

	for (int i = 0; i < 3; i++) printf("%d\n", a[i]);
	
	//2. Array
	int arr[3] = { 3,1,4 }; 
	// 4byte¥���� 3�� == 12byte
	// int�� 4byte == 4byte
	// sizeof(arr)/sizeof(int) == �迭�� ����

	for (int i = 0; i < sizeof(arr)/sizeof(int); i++) printf("%d\n", arr[i]);

	
	//3. Array�� Ȱ��1 : �Է¹��� ���ڵ��� �Ųٷ� ���
	int n;
	int arr[1000];

	printf("�Է��� ������ ���� �Է� : ");
	scanf_s("%d", &n);

	for (int i = 0; i < n; i++) scanf_s("%d", &arr[i]);

	for (int i = n - 1; i >= 0; i--) printf("%d ", arr[i]);
	
	//4. Array�� Ȱ��2 : �ִ� �ּ� ���ϱ�
	int n;
	int arr[100];

	scanf_s("%d", &n);

	for (int i = 0; i < n; i++) scanf_s("%d", &arr[i]);

	int max = arr[0];
	for (int i = 1; i < n; i++)
	{
		if (max < arr[i]) max = arr[i];
	}
	printf("max�� : %d\n", max);
	*/
	//5. ¦���� ����
	int n;
	int arr[100];

	scanf_s("%d", &n);

	for (int i = 0; i < n; i++) scanf_s("%d", &arr[i]);

	int cnt = 0;
	for (int i = 0; i < n; i++)
	{
		if (arr[i] % 2 == 0)
		{
			cnt++;
		}
	}
	printf("¦���� ������ : %d\n", cnt);


}