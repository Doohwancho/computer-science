#include <algorithm>
#include <vector>
#include <iostream>

/*

Runtime: 12 ms, faster than 83.43% of C++ online submissions for Build Array from Permutation.
Memory Usage: 16.3 MB, less than 44.77% of C++ online submissions for Build Array from Permutation.

*/

using namespace std;


vector<int> buildArray(vector<int>& nums) {
    vector<int> rst(nums.size());
    for (int i = 0; i < nums.size(); i++) {
        rst[i] = nums[nums[i]];
    }
    return rst;
}

int main() {

}


