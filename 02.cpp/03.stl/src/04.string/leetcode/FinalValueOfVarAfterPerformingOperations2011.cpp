#include <algorithm>
#include <vector>
#include <iostream>

/*

Runtime: 12 ms, faster than 47.12% of C++ online submissions for Final Value of Variable After Performing Operations.
Memory Usage: 14 MB, less than 92.67% of C++ online submissions for Final Value of Variable After Performing Operations.

*/

using namespace std;

class Solution {
public:
    int finalValueAfterOperations(vector<string>& operations) {
        int rst = 0;
        for (int i = 0; i < operations.size(); i++) {
            if (operations[i].at(0) == '+' || operations[i].at(2) == '+') {
                rst++;
            }
            else {
                rst--;
            }
        }
        return rst;
    }
};


/*
* 

Runtime: 8 ms, faster than 78.29% of C++ online submissions for Final Value of Variable After Performing Operations.
Memory Usage: 13.9 MB, less than 92.67% of C++ online submissions for Final Value of Variable After Performing Operations.

solution by cygnus

아 그냥 가운데꺼만 봐도 됬었네 

*/


class Solution {
public:
    int finalValueAfterOperations(vector<string>& operations) {
        int res = 0;
        for (auto operation : operations)
        {
            res += operation[1] == '+' ? 1 : -1;
        }
        return res;
    }
};