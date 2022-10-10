#include <stdio.h>

#define PI 3.14159 //매크로
#define PRINT_HELLO printf("Hello, World!\n")
#define SQUARE(X) ((X) * (X))
//printf("%d\n", 100 / SQUARE(a)); // = 100 / 5 * 5 
//printf("%d\n", SQUARE(a - 1)); // a-1*a-1 = a-a-1 = -1
//미리 ()쳐둠
#define MAX(A,B) (((A > B)) ? (A) : (B))
#define FOR(I,S,E) for (int I = S; I <= E; I++)
#define LOOP while(true)

/*
enum EGameState { //enum으로 묶어주면 일일이 숫자로 지정해줄 수고 덜음
	GAMESTATE_MAINMENU,
	GAMESTATE_PLAYING,
	GAMESTATE_PAUSED,
	GAMESTATE_GAMEOVER,
}

//const int GAMESTATE_MAINMENU = 0;
//const int GAMESTATE_PLAYING = 1;
//const int GAMESTATE_PAUSED = 2;
//const int GAMESTATE_GAMEOVER = 3;
*/


int main()
{
	//상수 : 변하지 않는 수
	//변수 : 변할 수 있는 수

	//상수 만드는 방법 3가지 : const, 매크로, enum

	//규칙 : 상수는 대문자로 쓴다

	/*
	//예제1 - const
	const float PI = 3.14159;

	float a = PI;
	float b = PI; //*= 2; //실수로 원래값 변경가능 -> const선언하면 애초에 파이 변경불가


	printf("pi = %.2f\n", PI);
	printf("&PI = %d\n", &PI); //상수는 메모리 안에 저장이 되어있음. 주소값도 가지고있음.
	

	//예제2 - 매크로(define을 이용한 상수선언)

	float a = PI;

	printf("pi = %.2f\n", PI);
	//printf("&PI = %d\n", &PI); //매크로로 선언하면 메모리에 저장안되서 주소값이 없다.
	

	//예제3 - enum

	//왜 사용?

	//const int GAMESTATE_MAINMENU = 0;
	//const int GAMESTATE_PLAYING = 1;
	//const int GAMESTATE_PAUSED = 2;
	//const int GAMESTATE_GAMEOVER = 3;

	//인덱실 불편해서 하나로 묶고 자동인덱싱 하기 위해

	int currState = 0;

	while (true)
	{
		switch (currState)
		{
		case GAMESTATE_MAINMENU:
			//
			break;
		case GAMESTATE_PLAYING:
			//
			break;
		case GAMESTATE_PAUSED:
			//
			break;
		case GAMESTATE_GAMEOVER:
			//
			break;
		}
	}
	

	//예제4 - 매크로 자세히 알아보기
	
	PRINT_HELLO;
	
	int a;
	scanf_s("%d", &a);
	//printf("%d\n", 100 / SQUARE(a)); // = 100 / 5 * 5 
	printf("%d\n", SQUARE(a-1)); // a-1*a-1 = a-a-1 = -1
	

	//예제5 - 매크로 자세히 알아보기2
	//3항연산자 : (a > b) ? a : b;
	
	int a = 5, b = 10;

	printf("%d\n", MAX(a, b));
	*/

	//매크로6 - 매크로 응용(Loop)

	FOR(i, 1, 10)
	{
		printf("%d",i);
	}

	LOOP{
		printf("*");
	}
}