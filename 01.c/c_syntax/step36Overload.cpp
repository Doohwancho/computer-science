#include <iostream>
using namespace std;


//overload
//같은 기능의 함수인데 형이 달라서 여러개 쓰는 경우, 이름 통일 가능
//컴파일러가 알아서 맞는 타입의 함수를 찾아서 넣어줌

//예제1
/*
void swap(int& a, int& b)
{
	int tmp = a;
	a = b;
	b = tmp;
}
void swap(double& a, double& b)
{
	int tmp = a;
	a = b;
	b = tmp;
}
void swap(int*(&a), int*(&b))
{
	int *tmp = a;
	a = b;
	b = tmp;
}


int main()
{
	int a = 20, b = 30;
	double da = 2.222, db = 3.333;
	int* pa = &a, * pb = &b;

	swap(a, b);
	swap(da, db);
	swap(pa, pb);

	cout << "a : " << a << endl;
	cout << "b : " << b << endl;

	cout << "da : " << da << endl;
	cout << "db : " << db << endl;

	cout << "*pa : " << *pa << endl;
	cout << "*pb : " << *pb << endl;

	return 0;
}
*/

//예제2

int inventory[20] = { 0 };
int score = 0;

//default값 선언으로 overloading 간소화
//default값은 가장 오른쪽부터 선언
void getItem(int itemId, int cnt = 1, int sc = 0)
{
	inventory[itemId] += cnt;
	score += sc;
}

int main()
{
	getItem(6,5);
	getItem(3,2);
	getItem(3);
	getItem(11,34,7000);

	cout << score << endl;
	for (int i = 0; i < 20; i++)
	{
		cout << inventory[i] << ' ';
	}
	cout << endl;
}