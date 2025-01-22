#include <stdio.h>
int main() {
	int arr[100][100] = { 0, };
	int i, j, k, x, y, cnt, N;
	i = j = k = x = y = cnt = N = 0;

	scanf("%d", &N);

	for (i = 0; i < N; i++) {
		scanf("%d %d", &x, &y);
		for (j = x - 1; j < x - 1 + 10; j++)
			for (k = y - 1; k < y - 1 + 10; k++)
				arr[j][k] = 1;
	}

	for (i = 0; i < 100; i++)
		for (j = 0; j < 100; j++)
			if (arr[i][j] == 1)
				cnt++;

	printf("%d", cnt);
	return 0;
}
