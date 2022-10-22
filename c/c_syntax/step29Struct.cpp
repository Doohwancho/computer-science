
#include <stdio.h>
/*
int main()
{

	//예제1

	typedef int Pair[2];

	Pair p; //int p[2];
	p[0] = 3;
	p[1] = 4;

	//위 코드는 직관적이지 못함

	typedef struct { int x, y; } Pair;

	Pair p; // int p[2];
	p.x = 3;
	p.y = 4;

	//이게 더 직관적


	//예제2

	//typedef struct { int x, y; } Point;
	struct Point { int x, y; }; //메모리상에선 아무일도 안일어나고, 그냥 포인트는 x,y로 이루어진 애입니다라고만 알아둠.


	Point p; //메모리에선 int형이 들어갈 수 있는 4바이트 공간이 두개 만들어짐.
	// == struct { int x, y; } p;

	p.x = 3;
	p.y = 5;

	printf("(%d,%d)\n", p.x, p.y);
	printf("sizeof(p) = %d\n", sizeof(p));
}
	

//예제3 - 구조체와 메모리

struct ProductInfo {
	int num;       //4b
	char name[100];//100b
	int cost;      //4b
};

int main() {
	ProductInfo myProduct = { 242412, "제주 한라봉", 19990 };

	printf("상품 번호 : %d\n", &myProduct.num); //6158304
	printf("상품 이름 : %d\n", myProduct.name); //6158308
	printf("가     격 : %d원\n", &myProduct.cost); //6158408
	//결론: num,name,cost는 메모리상에 붙어서 저장된다.
	//mycro note자체의 주솟값도 6158304
	printf("%d\n", &myProduct);

	printf("sizeof(myProduct) = %d\n", sizeof(myProduct));
}


//예제4 - 구조체 포인터

struct ProductInfo {
	int num;       //4b
	char name[100];//100b
	int cost;      //4b
};

//myProduct.cost -= myProduct.cost * 10 / 100;
void productSale(ProductInfo *p, int percent) //*p와 ->는 call-by-value아닌 call-by-reference
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
	ProductInfo myProduct = { 242412, "제주 한라봉", 20000 };
	ProductInfo otherProduct = { 153232, "성주 꿀참외", 10000 };

	//ProductInfo *ptr_product = &myProduct;
	//productSale(&myProduct, 10);
	productSwap(&myProduct, &otherProduct);

	printf("상품 번호 : %d\n", myProduct.num);
	//printf("상품 번호 : %d\n", (*ptr_product).num);
	//printf("상품 번호 : %d\n", ptr_product -> num);//(*a).b == a->b

	printf("상품 이름 : %s\n", myProduct.name);
	//printf("상품 이름 : %s\n", (*ptr_product).name); 
	//printf("상품 이름 : %s\n", ptr_product->name);

	printf("가     격 : %d원\n", myProduct.cost);
	//printf("가     격 : %d원\n", (*ptr_product).cost);
	//printf("가     격 : %d원\n", ptr_product->cost);
}

//예제5 - 구조체에 함수 집어넣기

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

//예제6

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