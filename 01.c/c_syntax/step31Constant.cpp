#include <stdio.h>

#define PI 3.14159 //��ũ��
#define PRINT_HELLO printf("Hello, World!\n")
#define SQUARE(X) ((X) * (X))
//printf("%d\n", 100 / SQUARE(a)); // = 100 / 5 * 5 
//printf("%d\n", SQUARE(a - 1)); // a-1*a-1 = a-a-1 = -1
//�̸� ()�ĵ�
#define MAX(A,B) (((A > B)) ? (A) : (B))
#define FOR(I,S,E) for (int I = S; I <= E; I++)
#define LOOP while(true)

/*
enum EGameState { //enum���� �����ָ� ������ ���ڷ� �������� ���� ����
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
	//��� : ������ �ʴ� ��
	//���� : ���� �� �ִ� ��

	//��� ����� ��� 3���� : const, ��ũ��, enum

	//��Ģ : ����� �빮�ڷ� ����

	/*
	//����1 - const
	const float PI = 3.14159;

	float a = PI;
	float b = PI; //*= 2; //�Ǽ��� ������ ���氡�� -> const�����ϸ� ���ʿ� ���� ����Ұ�


	printf("pi = %.2f\n", PI);
	printf("&PI = %d\n", &PI); //����� �޸� �ȿ� ������ �Ǿ�����. �ּҰ��� ����������.
	

	//����2 - ��ũ��(define�� �̿��� �������)

	float a = PI;

	printf("pi = %.2f\n", PI);
	//printf("&PI = %d\n", &PI); //��ũ�η� �����ϸ� �޸𸮿� ����ȵǼ� �ּҰ��� ����.
	

	//����3 - enum

	//�� ���?

	//const int GAMESTATE_MAINMENU = 0;
	//const int GAMESTATE_PLAYING = 1;
	//const int GAMESTATE_PAUSED = 2;
	//const int GAMESTATE_GAMEOVER = 3;

	//�ε��� �����ؼ� �ϳ��� ���� �ڵ��ε��� �ϱ� ����

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
	

	//����4 - ��ũ�� �ڼ��� �˾ƺ���
	
	PRINT_HELLO;
	
	int a;
	scanf_s("%d", &a);
	//printf("%d\n", 100 / SQUARE(a)); // = 100 / 5 * 5 
	printf("%d\n", SQUARE(a-1)); // a-1*a-1 = a-a-1 = -1
	

	//����5 - ��ũ�� �ڼ��� �˾ƺ���2
	//3�׿����� : (a > b) ? a : b;
	
	int a = 5, b = 10;

	printf("%d\n", MAX(a, b));
	*/

	//��ũ��6 - ��ũ�� ����(Loop)

	FOR(i, 1, 10)
	{
		printf("%d",i);
	}

	LOOP{
		printf("*");
	}
}