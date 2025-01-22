
//solution1

//Runtime: 0 ms, faster than 100.00 % of C++ online submissions for Construct the Rectangle.
//Memory Usage : 6 MB, less than 72.38 % of C++ online submissions for Construct the Rectangle.

//아 이제 슬슬 난이도 올려야 되나

class Solution {
public:
    vector<int> constructRectangle(int area) {
        vector<int> rst;
        int cap = (int)ceil(sqrt(area));

        for (int i = cap; i > 0; i--) {
            if (area % i == 0) {
                if (i >= area / i) {
                    rst.push_back(i);
                    rst.push_back(area / i);
                }
                else {
                    rst.push_back(area / i);
                    rst.push_back(i);
                }
                return rst;
            }
        }
        return rst;
    }
};