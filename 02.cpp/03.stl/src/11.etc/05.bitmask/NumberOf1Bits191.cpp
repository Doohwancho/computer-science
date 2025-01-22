//solution1

//Runtime: 3 ms, faster than 51.46 % of C++ online submissions for Number of 1 Bits.
//Memory Usage : 6 MB, less than 48.08 % of C++ online submissions for Number of 1 Bits.



//1000 0000 0000 0000 0000 0000 0000 0000

//32bit�� ���� ���� bit Ű��, 

//��ĭ�� ���������� ����鼭 ���� n�� &���� ���ִ� ���

//O(1)�̶� �翬�� 100%�� �� �˾Ҵµ�, �� �ƴ���?

//�̰ͺ��� �� ���� ����� ���� �� �ֳ�? 

//����ȭ�� �����Լ� ���°� ���� ����� ���̴µ�

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

//�� ���Ƴ�

//solution1�� O(1)�̶� ����Ǹ� ����ϰ��� �����ߴµ�, �̰� �� ����ȭ �ϴ� ����� �־���

//32bit ������ ���ƾ� �ϴµ�, ���⼭ ������ bit�� ��ŵ�ϴ� ��

//�� loop���� -1�ϰ� ������ &�����ϸ�, ������ �� ���� ���� bit�� ���� �� ����. �װ� 0�� ������ ī��Ʈ �ϴ� ��

//1001 �̷���, 1000�� �Ǿ� ������ &�����ϸ� 1000�� ��.

//���� 1000 �̷�����, -1�ؼ� 0111�� �Ǿ�, ������ &�����ϸ� 0000�� ��

//��

int hammingWeight(uint32_t n) {
    int count = 0;

    while (n) {
        n &= (n - 1);
        count++;
    }

    return count;
}