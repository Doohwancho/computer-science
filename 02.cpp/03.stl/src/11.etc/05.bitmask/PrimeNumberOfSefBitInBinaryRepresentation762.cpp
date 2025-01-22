
//solution1

//Runtime: 8 ms, faster than 77.94% of C++ online submissions for Prime Number of Set Bits in Binary Representation.
//Memory Usage : 5.9 MB, less than 80.66 % of C++ online submissions for Prime Number of Set Bits in Binary Representation.

//�̰ź��� �� ���� ���� �ֳ�?

//�����Լ�����ϳ�?

//isPrime() -> O(N)

//numberOfBinary() -> O(1)

//numberOfBinary()�� O(1)�ϱ� �꺸�� O(N)�� isPrime()�� ���� ����� ã�ƾ� �ڳ�


class Solution {
public:
    bool isPrime(int n) {
        if (n == 0 || n == 1) return false;

        for (int i = 2; i <= n / 2; ++i) {
            if (n % i == 0) {
                return false;
            }
        }
        return true;
    }

    int numberOfBinary(int n) {
        int count = 0;
        while (n) {
            count += n & 1;
            n >>= 1;
        }
        return count;
    }

    int countPrimeSetBits(int left, int right) {
        int rst = 0;
        for (int i = left; i <= right; i++) {
            if (isPrime(numberOfBinary(i))) {
                rst++;
            }
        }
        return rst;
    }
};



//solution2

//Runtime: 8 ms, faster than 77.46% of C++ online submissions for Prime Number of Set Bits in Binary Representation.
//Memory Usage : 6 MB, less than 49.71 % of C++ online submissions for Prime Number of Set Bits in Binary Representation.


//isPrime() �ٲ�

//�������� 8ms 12ms �����ư��鼭 ���Դµ�, ��� 8ms �����ϰ� ����

//2�� ��� �Ѱܼ� �׷��� ��

//�ٵ� �ƹ��� �׷��ٰ� �ص� isPrime()�� O(N)���� �� �������� �ʾ���


class Solution {

public:
    bool isPrime(int n) {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        for (int i = 3; (i * i) <= n; i += 2) {
            if (n % i == 0) return false;
        }
        return true;
    }

    int numberOfBinary(int n) {
        int count = 0;
        while (n) {
            count += n & 1;
            n >>= 1;
        }
        return count;
    }

    int countPrimeSetBits(int left, int right) {
        int rst = 0;
        for (int i = left; i <= right; i++) {
            if (isPrime(numberOfBinary(i))) {
                rst++;
            }
        }
        return rst;
    }
};

//solution3 by alexandar

//Runtime: 28 ms, faster than 47.25% of C++ online submissions for Prime Number of Set Bits in Binary Representation.
//Memory Usage : 6.1 MB, less than 29.19 % of C++ online submissions for Prime Number of Set Bits in Binary Representation.


//��¥�� int���� 32bit�̶����� �̿��ؼ�, �ƹ��� Ŀ������, prime number�� 32�� �ʰ��� �� ����.

//�ٵ� solution2�� �̹������ �� ������ ����.

//�� ����� �����ϱ� �ѵ� �� �������� ���ٰ� �ϳ��� funciton���� ���� ����� «������ �������ݾ�. ù���� ����� �̰� ���� ��?�ҵ�



class Solution {
public:
    int countPrimeSetBits(int l, int r) {
        set<int> primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
        int cnt = 0;
        for (int i = l; i <= r; i++) {
            int bits = 0;
            for (int n = i; n; n >>= 1)
                bits += n & 1;
            cnt += primes.count(bits);
        }
        return cnt;
    }
};


//solution4 by StefanPochmann


//Runtime: 4 ms, faster than 96.39 % of C++ online submissions for Prime Number of Set Bits in Binary Representation.
//Memory Usage : 5.9 MB, less than 49.71 % of C++ online submissions for Prime Number of Set Bits in Binary Representation.


//�� �� �̰� ����?

//__builtin_popcount()�� bit1�� ���� ��ȯ

//665772�� ��Ʈ�� �ٲٸ� 0b10100010100010101100 �ε�,

//2nd,3rd,5th,7th,11th,13th,17th,19th,23rd and 29th bits �� 1�̰�(�Ҽ� ��°��) ������ 0��. 

//665772 >> __builtin_popcount(L++) �� ������ ���� �����

//���� 5�� prime number���� Ȯ���ϰ� ������,

//665772�� 0b10100010100010101100 �긦 ���������� 5�� ����

//0b00000101000101000101 �׷� �̷��� �Ǵµ�, �� ������° ��(5����)�� 1���� &1�� Ȯ���ϴ� ��

//�����


class Solution {

public:
    int countPrimeSetBits(int L, int R) {
        int count = 0;
        while (L <= R)
            count += 665772 >> __builtin_popcount(L++) & 1;
        return count;
    }
};