#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

/*
 * Q. why does bit-vector sort?
 *
 * programming pearls ch1의 문제의 핵심은 sort시 1MB의 작은 RAM memory 밖에 할당되지 않음.
 *
 * 80MB의 .txt 파일을 1MB의 자료구조에 우겨넣을 수 있어야, 한번에 RAM에 올려서 sort 처리 가능
 *
 * bit-vector는 memory-efficient한 자료구조
 *
 */

#define N	10000000 //천만개 rows의 데이터를 저장해야 한다.
#define K	10000000 //천만개 rows의 데이터를 읽어와야 한다.
#define SHIFT	5    //int 사이즈가 32bit인데, 2^5 = 32이라 5번 right shift할거라 5
#define MASK	0x1f //31 in decimal,
                     //0001 1111,
                     //2^0 = 1 + 2^1 = 2 + 2^2 = 4 + 2^3 = 8 + 2^4 = 16 = 31,
                     //used to select a specific bit within those chun


static int	arr[(N >> SHIFT) + 1]; //천만개 숫자를 bit로 저장하려면 천만 bit만 필요하다.
                                   //그런데 int arr[10,000,000]를 하면, 1 int당 4byte, 32bit이니까, 40,000,000 byte나 320,000,000bit를 쓰게된다.
                                   //우린 10,000,000 bit만 필요하니까, 3억 2천만 bit에서 32를 나눠서 천만 bit를 구한다.
                                   //그러기 위해서 >> 5 (divide by 2^5)를 해준다.
                                   //그러나 유의할 점은, 여전히 int arr[]이기 때문에, 쪼개는 단위는 32bit int이다.
                                   //그러면 31만 2501개의 int가 들어가는 array 메모리 공간이 할당된다.
                                   //이게 왜 중요하냐면, 추후 7 digit 숫자를 넣을 때, shift 연산으로, 몇번째 int에 넣을지 정해야 하기 때문이다.
                                   //맨 마지막 + 1은 segfault 방지용 여유분 메모리다.
                                   //여담으로, static으로 선언된 애는 heap에 저장되는 애들과는 다르게 dis-allocate memory할 필요 없이 os가 프로그램 실행 끝나면 알아서 메모리 해제 해준다.






#define setbit(x, i)	(x[i >> SHIFT] |= (1 << (i & MASK))) //꺼낸 7 digit 양수가 i
/*
`
Q. 7 digit 숫자를 넣으려면?
A.
1. 저 int arr[]서, int단위로 총 31만 2501개씩 쪼개져 있는데, 이 int단위로 쪼개진 것 중에, 1,234,567가 몇번째 bit인지 구하면 된다.
    - 그러기 위해서 i >> SHIFT (2^5 = 32bit, which is size of int)를 하여 몇번째 index에 bit을 체크할지 구해준다.
2. 1,234,567 >> 5 = 38,580th 인덱스의 int를 찾았으면, 해당 32bit 에서 몇번째 bit가 1,234,567에 해당하는지 찾아 0->1 bit를 켜준다.
    - 그러기 위해서는, 1,234,567를 32bit로 나눈 나머지 값, remainder를 구하면 된다.
    - 1,234,567 % 32로도 구할 수 있지만, 더 빠른 bitmask로 나머지를 구하는 방법은 다음과 같다.
    - 1,234,567 in Binary is 100101101011010000111
    - 0x1F in binary is 11111
    - 1,234,567 & 0x1F is 00111 = 7
    - 1,234,567 & 0x1f (which is 0000 0000 0000 0000 0000 0000 0001 1111)하면, 7이 나오고, 해당 38,580th 인덱스에서 7번째 bit를 켜면 그 비트가 전체적으로는 1,234,567 bit라는 걸 알 수 있다.
3. setbit macro: arr[38,580] |= (1 << 7). 이 되는 것이다.
*/

#define clrbit(x, i)	(x[i >> SHIFT] &= ~(1 << (i & MASK)))
#define getbit(x, i)	(x[i >> SHIFT] & (1 << (i & MASK)))

int main(void)
{
	int	i, t;
	FILE	*fp;

	bzero(arr, sizeof(arr));
	fp = fopen("../shuffled_numbers.txt", "r");
	for (i = 0; i < K; i++) {
		fscanf(fp, "%d", &t);
		setbit(arr, t); //t는 꺼낸 7 digit 양수
	}
	fclose(fp);

	fp = fopen("./sort.txt", "w");
	for (i = 0; i < N; i++) {
		if (getbit(arr, i))
			fprintf(fp, "%d\n", i);
	}
	fclose(fp);
	return(0);
}
