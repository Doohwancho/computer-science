
//solution1

//Runtime: 8 ms, faster than 77.94% of C++ online submissions for Prime Number of Set Bits in Binary Representation.
//Memory Usage : 5.9 MB, less than 80.66 % of C++ online submissions for Prime Number of Set Bits in Binary Representation.

//이거보다 더 빠를 수가 있나?

//내장함수써야하나?

//isPrime() -> O(N)

//numberOfBinary() -> O(1)

//numberOfBinary()는 O(1)니까 얘보다 O(N)인 isPrime()을 줄일 방법을 찾아야 겠네


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


//isPrime() 바꿈

//이전꺼는 8ms 12ms 번갈아가면서 나왔는데, 얘는 8ms 일정하게 나옴

//2의 배수 넘겨서 그런가 봄

//근데 아무리 그렇다고 해도 isPrime()이 O(N)에서 더 나아지진 않았음


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


//어짜피 int형이 32bit이란점을 이용해서, 아무리 커져봤자, prime number가 32를 초과할 수 없음.

//근데 solution2가 이방법보단 더 마음에 들음.

//이 방법은 참신하긴 한데 비 직관적인 데다가 하나의 funciton에서 여러 기능을 짬뽕으로 수행하잖아. 첫보는 사람이 이걸 보면 잉?할듯



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


//와 씨 이건 뭐냐?

//__builtin_popcount()가 bit1의 갯수 반환

//665772가 비트로 바꾸면 0b10100010100010101100 인데,

//2nd,3rd,5th,7th,11th,13th,17th,19th,23rd and 29th bits 가 1이고(소수 번째만) 나머진 0임. 

//665772 >> __builtin_popcount(L++) 이 연산이 먼저 수행됨

//숫자 5가 prime number인지 확인하고 싶으면,

//665772인 0b10100010100010101100 얘를 오른쪽으로 5번 땡김

//0b00000101000101000101 그럼 이렇게 되는데, 맨 마지막째 번(5번쨰)가 1인지 &1로 확인하는 것

//놀랍네


class Solution {

public:
    int countPrimeSetBits(int L, int R) {
        int count = 0;
        while (L <= R)
            count += 665772 >> __builtin_popcount(L++) & 1;
        return count;
    }
};