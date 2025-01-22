#include <algorithm>
#include <vector>
#include <iostream>

/*
* 
 

Runtime: 72 ms, faster than 5.05% of C++ online submissions for Decode XORed Array.
Memory Usage: 24.8 MB, less than 93.72% of C++ online submissions for Decode XORed Array.

*/

using namespace std;


class Solution {
public:
    vector<int> decode(vector<int>& encoded, int first) {
        int len = encoded.size();
        vector<int> orig(len + 1);
        orig[0] = first;
        for (int i = 1; i < len + 1; i++) {
            orig[i] = orig[i - 1] ^ encoded[i - 1];
        }
        return orig;
    }
};


//solution by it_bilim

//Runtime: 62 ms, faster than 5.05 % of C++ online submissions for Decode XORed Array.
//Memory Usage : 26.1 MB, less than 29.71 % of C++ online submissions for Decode XORed Array.


// ���� ����� �̸� �����ϰ� �ٽ� index0���� for loop���� �����ϴ� �� ����
//���� �迭�� �����ϰ�, �ǽð����� �����鼭 �迭������ �ø��°� 10ms�� ��������� �ִ�.


class Solution {
public:
    vector<int> decode(vector<int>& encoded, int first) {
        vector<int> ans;
        ans.push_back(first);
        for (int i = 0; i < encoded.size(); i++) {
            ans.push_back(ans.back() ^ encoded[i]);
        }
        return ans;

    }
};