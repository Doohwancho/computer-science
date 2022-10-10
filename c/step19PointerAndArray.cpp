#include <stdio.h>

int main()
{
	/*
	//예제1 - 배열 포인터 : 배열을 가리키는 포인터
	int arr[3] = { 1,2,3 };

	int(*ptr_arr)[3]; //길이 3인 int형 배열을 가리키는 포인터를 선언
	ptr_arr = &arr;

	for (int i = 0; i < 3; i++)
	{
		printf("%d\n", (*ptr_arr)[i]);
	}
	
	//예제2 - 2차원 배열과 배열포인터
	int arr[2][3] = { {1,2,3},{4,5,6} };

	printf("%d\n", sizeof(arr)); //24
	printf("%d\n", sizeof(arr[0])); //12 
	printf("%d\n", sizeof(arr[0][0])); //4

	printf("%d\n", &arr);  //셋 다 모두 같은 값(arr[0]의 주솟값)
	printf("%d\n", &arr[0]);  
	printf("%d\n", &arr[0][0]);
	
	
	//예제3 - 2차원 배열 포인터

	int arr[2][3] = { {1,2,3},{4,5,6} };
	
	int(*ptr)[3] = arr;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%d", ptr[i][j]);
			printf("%d",sizeof(ptr[i]));   //12 - arr[0]의 전체 int의 사이즈
			printf("%d", sizeof(ptr[i][j])); //4 - int의 사이즈
		}
		printf("\n");
	}
	*/
	//예제4 -- 배열포인터 응용

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