/*
	포인터(pointer)
	변수를 가리키는 애 - 변수의 주소를 저장하는 변수
	
	예를들어, a=20은 램 어딘가에 a라는 변수가 20을 가지고 있다고 저장되어있음.
	포인터도 램 어딘가에 저장되어 있는 변수임.
	a가 20인걸 알려면 a를 포인터가 찾아야 함.
	어떻게 찾냐면 a의 주소값으로 찾음
	램 안에는 수많은 변수들이 저장되어 있기 때문에, a를 저장할 때도 a의 자리의 주소값이 있음
	예를들어 1125번째 자리이다.
	이 1125번째 자리이다라는 것을 기준으로 포인터가 a를 찾음
*/

#include <stdio.h>

int main() 
{
	/*
	//예제1
	int a = 20;
	
	int *ptr_a; //가리킬 변수의 형 + 별표

	ptr_a = &a; //&는 주소값을 뜻함. ex) &main은 메인의 주솟값임.

	printf("%d\n", ptr_a); //10157184 , 9436640 , ... 
	//실행할 때마다 달라짐. 왜? 실행할 때마다 램의 상황이 달라지기 때문

	printf("a의 값 : %d\n", a); 
	printf("a의 주솟값 : %d\n", &a); //a의 주솟값
	printf("ptr_a에 저장된 값 : %d\n", ptr_a); //포인터가 가리키는 변수의 주솟값
	printf("ptr_a가 가리키는 변수의 값 : %d\n", *ptr_a); //*은 ptr_a가 가리키는 변수 자체를 사용하고 싶을 때 (==a)
	

	//예제2
	int a = 10;
	int b = 20;

	int* ptr;

	ptr = &a;
	printf("ptr이 가리키는 변수에 저장된 값 : %d\n", *ptr);

	ptr = &b;
	printf("ptr이 가리키는 변수에 저장된 값 : %d\n", *ptr);
	

	//예제3
	int a = 10;

	int* ptr;
	ptr = &a;

	printf("a의 값 : %d\n", a);

	*ptr = 20; //a의 주솟값에 20을 저장
	
	printf("a의 값 : %d\n", a);
	

	//예제4
	int a = 10;

	int* ptr;
	ptr = &a;

	int** ptr_ptr;
	ptr_ptr = &ptr;

	printf("a = %d\n", a); //a의 값
	printf("&a = %d\n", &a); //a의 주솟값
	
	printf("ptr = %d\n", ptr); //a의 주솟값
	printf("&ptr = %d\n", &ptr); //a의 주솟값을 저장하고 있는 ptr의 주솟값

	printf("ptr_ptr = %d\n", ptr_ptr); //a의 주솟값을 저장하고 있는 ptr의 주솟값
	printf("*ptr_ptr = %d\n", *ptr_ptr); //a의 주솟값
	printf("**ptr_ptr = %d\n", **ptr_ptr); //a의 값

	
	//예제5
	int a = 10;
	int* ptr_a;
	ptr_a = &a;


	printf("ptr_a의 값 : %d\n", ptr_a); //9698224
	printf("ptr_a+1의 값: %d\n", ptr_a + 1); //9698228 -- 왜 +1이 아니고 +4?
	//컴퓨터(램)는 1바이트가 가장 작은 처리단위.(1바이트는 8비트. 2진수가 들어가는게 8개)
	//int는 4바이트. +1하면 +4바이트.
	//그래서 포인터에 +n하면, 포인터가 가리키고있는 형의 크기만큼 더함

	//예제6
	int arr[5] = { 1,2,3,4,5 };

	printf("arr의 값 : %d\n", arr); //7337936 == arr[0] -> arr == &arr[0]
	for (int i = 0; i < 5; i++)
	{
		//int를 4바이트씩 나눠서 메모리에 저장하는구나
		printf("&arr[%d] = %d\n", i, &arr[i]); //7337936  7337940  7337944  7337948  7337952

		printf("arr + %d = %d\n", i, arr + i);
	}
	

	//예제7
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

	for (int* ptr = arr; ptr < arr + 10; ptr++) //여기서 ptr++은 int형이기에 +4이고, +10은 40임
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
	//왜?
	//컴퓨터는 a[b] == *(a+b)로 인식하기 때문에
	//a+b를 b+a로 바꾸어 b[a]도 같은 결과값이 나온다.
	//개싱기
	*/
	/*
		정리

		1. arr == &arr[0] (ptr == &ptr[0])
		2. *ptr == ptr[0]
		3. ptr + 1 == ptr에 siszeof(*ptr)을 더한 값
	*/
	//예제8
	int arr[4] = { 1,2,3,4 };

	printf("arr = %d\n", arr);
	printf("arr + 1 = %d\n", arr + 1); //+4 arr[0]인 sizeof(int)만큼 4증가

	printf("&arr = %d\n", &arr);
	printf("&arr + 1 = %d\n", &arr + 1); //+12 왜? sizeof(array)만큼 4*n증가

}