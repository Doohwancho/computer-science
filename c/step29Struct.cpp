
#include <stdio.h>
/*
int main()
{

	//����1

	typedef int Pair[2];

	Pair p; //int p[2];
	p[0] = 3;
	p[1] = 4;

	//�� �ڵ�� ���������� ����

	typedef struct { int x, y; } Pair;

	Pair p; // int p[2];
	p.x = 3;
	p.y = 4;

	//�̰� �� ������


	//����2

	//typedef struct { int x, y; } Point;
	struct Point { int x, y; }; //�޸𸮻󿡼� �ƹ��ϵ� ���Ͼ��, �׳� ����Ʈ�� x,y�� �̷���� ���Դϴٶ�� �˾Ƶ�.


	Point p; //�޸𸮿��� int���� �� �� �ִ� 4����Ʈ ������ �ΰ� �������.
	// == struct { int x, y; } p;

	p.x = 3;
	p.y = 5;

	printf("(%d,%d)\n", p.x, p.y);
	printf("sizeof(p) = %d\n", sizeof(p));
}
	

//����3 - ����ü�� �޸�

struct ProductInfo {
	int num;       //4b
	char name[100];//100b
	int cost;      //4b
};

int main() {
	ProductInfo myProduct = { 242412, "���� �Ѷ��", 19990 };

	printf("��ǰ ��ȣ : %d\n", &myProduct.num); //6158304
	printf("��ǰ �̸� : %d\n", myProduct.name); //6158308
	printf("��     �� : %d��\n", &myProduct.cost); //6158408
	//���: num,name,cost�� �޸𸮻� �پ ����ȴ�.
	//mycro note��ü�� �ּڰ��� 6158304
	printf("%d\n", &myProduct);

	printf("sizeof(myProduct) = %d\n", sizeof(myProduct));
}


//����4 - ����ü ������

struct ProductInfo {
	int num;       //4b
	char name[100];//100b
	int cost;      //4b
};

//myProduct.cost -= myProduct.cost * 10 / 100;
void productSale(ProductInfo *p, int percent) //*p�� ->�� call-by-value�ƴ� call-by-reference
{
	p->cost -= p->cost * percent / 100;
}

void productSwap(ProductInfo* a, ProductInfo* b)
{
	ProductInfo tmp = *a;
	*a = *b;
	*b = tmp;
}


int main() {
	ProductInfo myProduct = { 242412, "���� �Ѷ��", 20000 };
	ProductInfo otherProduct = { 153232, "���� ������", 10000 };

	//ProductInfo *ptr_product = &myProduct;
	//productSale(&myProduct, 10);
	productSwap(&myProduct, &otherProduct);

	printf("��ǰ ��ȣ : %d\n", myProduct.num);
	//printf("��ǰ ��ȣ : %d\n", (*ptr_product).num);
	//printf("��ǰ ��ȣ : %d\n", ptr_product -> num);//(*a).b == a->b

	printf("��ǰ �̸� : %s\n", myProduct.name);
	//printf("��ǰ �̸� : %s\n", (*ptr_product).name); 
	//printf("��ǰ �̸� : %s\n", ptr_product->name);

	printf("��     �� : %d��\n", myProduct.cost);
	//printf("��     �� : %d��\n", (*ptr_product).cost);
	//printf("��     �� : %d��\n", ptr_product->cost);
}

//����5 - ����ü�� �Լ� ����ֱ�

struct Time 
{
	int h, m, s;

	int totalSec()
	{
		return 3600 * h + 60 * m + s;
	}
};

int main()
{
	Time t = { 1,22,48 };

	printf("%d\n", t.totalSec());
}
*/

//����6

struct Point {
	int x, y;

	void moveRight() { x++; }
	void moveLeft() { x--; }
	void moveUp() { y++; }
	void moveDown() { y--; }
};

int main()
{
	Point p = { 0,0 };

	p.moveRight();
	p.moveDown();

	printf("(%d,%d)\n", p.x, p.y);
}