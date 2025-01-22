#include <vector>
#include <iostream>

using namespace std;

/*
* 
Runtime: 0 ms, faster than 100.00% of C++ online submissions for Concatenation of Array.
Memory Usage: 12.2 MB, less than 99.70% of C++ online submissions for Concatenation of Array.

*/

class Solution {
public:
    vector<int> getConcatenation(vector<int>& nums) {
        int len = nums.size();
        vector<int> v(len * 2);
        for (int i = 0, j = len; i < len; i++, j++) {
            v[i] = nums[i];
            v[j] = nums[i];
        }
        return v;
    }
};