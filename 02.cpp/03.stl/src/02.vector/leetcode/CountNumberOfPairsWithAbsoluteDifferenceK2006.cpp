#include <algorithm>
#include <vector>
#include <iostream>

/*
*

Runtime: 16 ms, faster than 79.69% of C++ online submissions for Count Number of Pairs With Absolute Difference K.
Memory Usage: 12.4 MB, less than 64.57% of C++ online submissions for Count Number of Pairs With Absolute Difference K.
*/

using namespace std;

class Solution {
public:
    int countKDifference(vector<int>& nums, int k) {
        int len = nums.size();
        int rst = 0;
        for (int i = 0; i < len - 1; i++) {
            for (int j = i + 1; j < len; j++) {
                if (abs(nums[i] - nums[j]) == k) {
                    rst++;
                }
            }
        }
        return rst;
    }
};