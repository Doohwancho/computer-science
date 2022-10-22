//prototype : 원형(원래 모양), 시제품
//용도:걷기 후 돌기 쓰다가 돌기 후 걷기 쓰고싶은데, C가 절차형이라 에러나는경우, 먼저 정의해놓고 나중에 값을 정의하는것
//따라서 함수들의 선언 순서에 대해 신경쓸 필요가 없고, 이 파일에 어떤 함수가 있는지 한눈에 알 수 있게 해준다.

#include <stdio.h>

//prototype
void walk(int);
void rotate(int);
void drawSquare();

//걷기
void walk(int distance)
{
	printf("%dcm를 걸었습니다.\n", distance);
}

//돌기
void rotate(int angle)
{
	printf("%d도 회전했습니다.\n", angle);
}

void drawSquare()
{
	for (int i = 1; i <= 4; i++)
	{
		walk(10);
		rotate(90);
	}
}


int main()
{
	drawSquare();
}