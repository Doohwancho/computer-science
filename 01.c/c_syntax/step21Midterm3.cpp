#include <stdio.h>

int main()
{
	/*
		//예제1

		//100개 이하의 정수를 입력받아
		//첫 줄에 짝수 번째 숫자들을 순서대로 출력하고,
		//다음 줄에 홀수 번째 숫자들을 순서대로 출력하는 프로그램을 만들어 보세요.

		//입력 예
		//7
		//3 1 4 1 5 9 2

		//출력 예
		//1 1 9
		//3 4 5 2
	
	
	int n;
	int arrNum[105];

	scanf_s("%d", &n);

	for (int i = 0; i < n; i++)
	{
		scanf_s("%d", &arrNum[i]);
	}
	
	for (int i = 1; i < n; i += 2)
	{
		printf("%d", arrNum[i]);
	}
	printf("\n");
	
	for (int i = 0; i < n; i += 2)
	{
		printf("%d", arrNum[i]);
	}
	printf("\n");
	

	//예제 2 - 코드를 보고 결과값 예측
	int a = 10;
	int b = 20;

	int* ptr;

	ptr = &a;
	*ptr = 30;

	ptr = &b;
	*ptr = 10;

	printf("%d\n", a); //30
	printf("%d\n", b); //10
	printf("%d\n", *ptr); //10
	

	//예제3 - 코드를 보고 결과값 예측

	int arr[10] = { 3,1,4,1,5,9,2,6,5,3 };

	printf("%d\n", arr); //3의 주솟값
	for (int i = 3; i < 7; i++) 
	{
		printf("%d %d\n", arr + i, *(arr + i)); //주솟값(1)1 주솟값(5)5 주솟값(9)9 주솟값(2)2
	}
	
	
	//예제4 
	//10*10 이하의 정수형 이차원 배열을 입력받아
	//그 배열의 각 행의 요소의 합을 출력하는 프로그램을 만들어 보세요

	//입력 예
	//3 4
	//4 2 6 3
	//7 9 3 4
	//5 1 2 1

	//출력 예
	//15
	//23
	//9
	

	int rst[11][11];

	int row, column;

	scanf_s("%d %d", &row, &column);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			scanf_s("%d", &rst[i][j]);
		}
	}

	for (int i = 0; i < row; i++)
	{
		int sum = 0;
		for (int j = 0; j < column; j++)
		{
			sum += rst[i][j];
		}
		printf("%d\n", sum);
	}
	
	
	//예제5 - 코드보고 결과값 예측

	int arr[3][3] = { 0 };

	//arr - &arr
	//row - arr, &arr[0]
	//each - *arr, arr[0]
	//?? - row랑 each는 무엇으로 구별되는 것인가?
	//each에서 *arr와 arr[0]에서의 arr는 전체 arr를 뜻하는게 아니라, row에서의 arr,즉 arr[0]을 뜻한다.
	//arr는 &arr[0]와 의미가 같고, *arr는 arr[0]과 의미가 같은데, 이것은 또다시 &arr[0][0]과 의미가 같겠죠?
	//&arr = &arr[0]  ( arr = arr[0] )

	printf("%d\n", &arr); // 100 (O) //arr전체를 가리킴
	printf("%d\n", arr); // 100  (0) //arr[0]을 가리킴
	printf("%d\n", *arr); // 0  (X) 100 //arr[0]을 가리킴 -- 완전 첫번째라고 명시하지 않으면 주솟값           //Question mark?!

	printf("%d\n", &arr[0]); // 100 (0)
	printf("%d\n", arr[0]); // 0  (X) 100 -- 완전 첫번째라고 명시하지 않으면 주솟값
	printf("%d\n", *arr[0]); // 0  (O) //arr[0]은 첫번째 arr의 주솟값의미. *은 값을 도출하라는 뜻.

	printf("%d\n", &arr[0][0]); //100 (0)

	printf("%d\n", arr[0][0]); //0 (0) -- 완전 첫번째라고 명시했기 때문에 그냥 값

	

	//예제6 - 코드보고 결과값 예측

	int arr[3][3] = { 0 };

	printf("%d\n", &arr[0][0]); //100 (0)

	printf("%d\n", arr[0] + 1); //100+24  (X) +4 //첫번째 배열 전체의 1번째 값
	printf("%d\n", *arr + 1); //100+12 (X) +4
	printf("%d\n", &arr[0] + 1); //100+12  (0) //2차원 배열 전체의 1번째 행
	printf("%d\n", arr + 1); //100+24 (X) +12
	printf("%d\n", &arr + 1); //100+24 (X) + 36(4*9) //&arr는 2차원 배열 전체를 가리킴
	
	*/

	//예제7 -- 주석처리된 코드에서 틀린 부분을 수정하라.

	int arr[3][4] = {
		{1,2,3,4},
		{5,6,7,8},
		{9,10,11,12}
	};

	int(*ptr)[4] = arr; // &arr[0] 주소를 집어넣음

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%d ", ptr[i][j]);
		}
		printf("\n");
	}
	
}