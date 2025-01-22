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


// 벡터 사이즈를 미리 지정하고 다시 index0부터 for loop으로 재탕하는 것 보다
//동적 배열로 선언하고, 실시간으로 넣으면서 배열사이즈 늘리는게 10ms더 성능향상이 있다.


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