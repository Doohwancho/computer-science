#include <vector>
#include <iostream>

using namespace std;

class Solution {
public:
    int maximumWealth(vector<vector<int>>& accounts) {
        if (accounts.size() == 0) return 0;
        int rst = accounts[0][0];
        for (int i = 0; i < accounts.size(); i++) {
            int rowSum = accounts[i][0];
            for (int j = 1; j < accounts[i].size(); j++) {
                rowSum += accounts[i][j];
            }
            if (rowSum > rst) {
                rst = rowSum;
            }
        }
        return rst;
    }
};