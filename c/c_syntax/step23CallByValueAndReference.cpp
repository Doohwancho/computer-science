// call-by-value : 값만 넘겨주고 끝난것
// call-by-reference : 값의 주소를 넘겨준것

#include <stdio.h>
/*
void swap(int *x, int *y)
{
	int tmp = *x;
	*x = *y;
	*y = tmp;
}
*/

void swap(int &x, int &y) //call by reference
{
	int tmp = x;
	x = y;
	y = tmp;
}


int main()
{
	int a, b;

	scanf_s("%d%d", &a, &b);

	swap(a, b);

	printf("a = %d, b = %d\n", a, b);
}