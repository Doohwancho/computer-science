#include <stdio.h>

//전역변수
int itemCnt = 0;
int money = 100;

void buyItem(int cost)
{
	//아이템을 산다.
	itemCnt++;
	money -= cost;
	printf("아이템을 구매했습니다.\n");
	printf(" 아이템 개수 : %d\n", itemCnt);
	printf(" 잔액 : %d\n", money);
}

int plus(int a, int b) //int type 반환
{
	return a + b;
}

int noMeaning()
{
	printf("first\n");
	return 1;
	printf("second\n"); //리턴을 만나면 즉시 함수 종료하기 때문에 2는 실행안됨.
	return 2;
}

void noMeaningtwo()
{
	printf("noMeaning\n");
	return; //void는 그냥 return;
}

int main()
{
	buyItem(30);

	buyItem(50);

	int sum = plus(3, 5);
	printf("%d\n", sum);

	int a;
	a = noMeaning();
	printf("반환된 값 : %d\n", a);

	noMeaningtwo();

	return 0; //명시하지 않아도 main은 return 0을 함.
	//왜? 0 이외의 값은 에러코드이기 때문. 쉘에 반환되는 값은 0이 TRUE이기 때문.
	//왜 굳이? 에러가 있을때 n을 반환하고 없을 때 0을 반환하면, 
	//if(main()) { 에러처리 } 시 0을 반환해야 정상이면 ==연산을 사용하지 않아도 됨.  
}