#include <stdio.h>

/*
//1. ���� ���α׷��� ��� �����?

typedef int Point[2];
typedef Point* PointPtr;

int main()
{
	Point p = { 3,4 }; // int p[2] = {3,4};
	PointPtr pp = &p;

	printf("%d %d %d\n", **pp, (*pp)[0], (*pp)[1]);//3 3 4
	//**pp == *(*pp) == *p == p[0]
	//(*pp)[0] == p[0]

}


//2. ��ĭ�� �� �ڵ��?

struct Point {
	int x, y;
};

void pSwap(Point* p)
{
	// x��ǥ�� y��ǥ�� ��ȯ
	int tmp;
	tmp = p->x;
	p->x = p->y;
	p->y = tmp;
}

int main()
{
	Point pos = { 3,4 };

	pSwap(&pos);

	printf("(%d,%d)\n", pos.x, pos.y);
}
*/

//3. pSwap�Լ��� Point ����ü�� ����ֱ�

struct Point {
	int x, y;

	void pSwap()
	{
		int tmp = x;
		x = y;
		y = tmp;
	}
};


int main()
{
	Point pos = { 3,4 };

	pos.pSwap();

	printf("(%d,%d)\n", pos.x, pos.y);
}