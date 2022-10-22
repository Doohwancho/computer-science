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
	// 4byte짜리가 3개 == 12byte
	// int는 4byte == 4byte
	// sizeof(arr)/sizeof(int) == 배열의 길이

	for (int i = 0; i < sizeof(arr)/sizeof(int); i++) printf("%d\n", arr[i]);

	
	//3. Array의 활용1 : 입력받은 숫자들을 거꾸로 출력
	int n;
	int arr[1000];

	printf("입력할 숫자의 갯수 입력 : ");
	scanf_s("%d", &n);

	for (int i = 0; i < n; i++) scanf_s("%d", &arr[i]);

	for (int i = n - 1; i >= 0; i--) printf("%d ", arr[i]);
	
	//4. Array의 활용2 : 최대 최소 구하기
	int n;
	int arr[100];

	scanf_s("%d", &n);

	for (int i = 0; i < n; i++) scanf_s("%d", &arr[i]);

	int max = arr[0];
	for (int i = 1; i < n; i++)
	{
		if (max < arr[i]) max = arr[i];
	}
	printf("max는 : %d\n", max);
	*/
	//5. 짝수의 갯수
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
	printf("짝수의 갯수는 : %d\n", cnt);


}