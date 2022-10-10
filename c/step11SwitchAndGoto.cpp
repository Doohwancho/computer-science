#include <stdio.h>

int main()
{
	int choice; 

	makeChoice: //goto

	printf("새 게임: 1\n");
	printf("불러오기 : 2\n");
	printf("설정 : 3\n");
	printf("크레딧 : 4\n");

	scanf_s("%d", &choice);

	/*
	if (choice == 1)
	{
		printf("새 게임\n");
	}
	else if (choice == 2)
	{
		printf("불러오기\n");
	}
	else if (choice == 3)
	{
		printf("설정\n");
	}
	else if (choice == 4)
	{
		printf("크레딧\n");
	}
	*/

	//switch문을 이용

	switch (choice)
	{
	case 1:
		printf("새 게임\n");
		break;             //break가 없으면, case1실행하고 case2로 넘어감.
	case 2:
		printf("불러오기\n");
		break;
	case 3:
		printf("설정\n");
		break;
	case 4:
		printf("크레딧\n");
		break;
	default:
		printf("잘못 입력하셨습니다. \n");
		goto makeChoice; //goto가 3개만 되도 코드가 꼬여버림 -> 스파게티코드
		break;
	}
}