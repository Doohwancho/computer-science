
//solution1

//Runtime: 0 ms, faster than 100.00 % of C++ online submissions for Binary Number with Alternating Bits.
//Memory Usage : 6 MB, less than 21.89 % of C++ online submissions for Binary Number with Alternating Bits.

//¿Íµû½Ã... ÃµÀé°¡?

class Solution {
public:
    bool hasAlternatingBits(int n) {
        long num = (n ^ (n >> 1));
        return ((num & (num + 1)) == 0);
    }
};