//solution1

//Runtime: 3 ms, faster than 51.46 % of C++ online submissions for Number of 1 Bits.
//Memory Usage : 6 MB, less than 48.08 % of C++ online submissions for Number of 1 Bits.



//1000 0000 0000 0000 0000 0000 0000 0000

//32bit에 제일 왼쪽 bit 키고, 

//한칸씩 오른쪽으로 땡기면서 원본 n과 &연산 해주는 방법

//O(1)이라 당연히 100%뜰 줄 알았는데, 왜 아니지?

//이것보다 더 빠른 방법이 있을 수 있나? 

//최적화된 내장함수 쓰는것 말곤 답없어 보이는데

class Solution {
public:
    int hammingWeight(uint32_t n) {
        int rst = 0;
        uint32_t a = 1 << 31;

        while (a > 0) {
            rst += ((n & a) > 0);
            a = a >> 1;
        }
        return rst;
    }
};



//solution 2 by housed

//Runtime: 0 ms, faster than 100.00% of C++ online submissions for Number of 1 Bits.
//Memory Usage : 5.8 MB, less than 79.46 % of C++ online submissions for Number of 1 Bits.

//와 미쳤네

//solution1이 O(1)이라 이쯤되면 충분하겠지 생각했는데, 이걸 더 최적화 하는 방법이 있었음

//32bit 무조건 돌아야 하는데, 여기서 안켜진 bit를 스킵하는 법

//매 loop마다 -1하고 원본과 &연산하면, 오른쪽 맨 끝에 켜진 bit만 지울 수 있음. 그게 0될 때까지 카운트 하는 것

//1001 이러면, 1000이 되어 원본과 &연산하면 1000이 됨.

//만약 1000 이랬으면, -1해서 0111이 되어, 원본과 &연산하면 0000이 됨

//오

int hammingWeight(uint32_t n) {
    int count = 0;

    while (n) {
        n &= (n - 1);
        count++;
    }

    return count;
}