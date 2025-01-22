
//첫인상: 뭔소리?

//if the number is consecutive like 0, 1, 2, 3, it forms a range 0->3
//if not, it forms multiple range.A range can be only one number

//ㅏ....


//solution1

//Runtime: 0 ms, faster than 100.00% of C++ online submissions for Summary Ranges.
//Memory Usage : 7 MB, less than 13.52 % of C++ online submissions for Summary Ranges.

//O(N)

//cpp에서는 (string)으로 형변환 안하고 std:to_string()으로 형변환 하는구나

//empty vector return하라길래 return new vector<string>; 했더니 에러남. 자바에선 잘 됬는데;;

//근데 또 신기한게 초기화 해준 후 리턴하면 잘 들어감. cpp에서는 new를 안썼는데도 메모리가 할당되나봄.

class Solution {

public:
    vector<string> summaryRanges(vector<int>& nums) {
        vector<string> rst;
        if (nums.size() == 0) return rst;
        string accum = std::to_string(nums[0]);

        for (int i = 0, j = 1; j < nums.size(); i++, j++) {
            if (nums[j] == nums[i] + 1) {
                if (j == nums.size()-1 || nums[j + 1] != nums[j] + 1) {
                    accum += "->" + std::to_string(nums[j]);
                }
            }
            else if (nums[i] == nums[j]) {
                rst.push_back(accum);
            }
            else {
                rst.push_back(accum);
                accum = std::to_string(nums[j]);
            }
        }
        rst.push_back(accum);
        return rst;
    }
};



//solution2

//Runtime: 0 ms, faster than 100.00 % of C++ online submissions for Summary Ranges.
//Memory Usage : 6.9 MB, less than 63.49 % of C++ online submissions for Summary Ranges.

//index가 0,1,2,3,4,... 이렇게 늘어나잖아?

//각 숫자 - 그 숫자 위치의 인덱스 했을 때, 같은수가 나오면 한 그룹으로 묶는 방법

//long을 쓴 건, int끼리 연산 시 오버플로우 나는 경우가 있기 때문.


//nums = [0, 2, 3, 4, 6, 8, 9]
//Output: ["0", "2->4", "6", "8->9"]

//nums = [0, 2, 3, 4, 6, 8, 9]
//indx = [0, 1, 2, 3, 4, 5, 6]
//diff = [0, 1, 1, 1, 2, 3, 3]

//rst  = [0, 2->4, 6, 8->9]


class Solution {

public:
    vector<string> summaryRanges(vector<int>& nums) {
        vector<string> rst;
        if (nums.size() == 0) return rst;
        vector<long> diff;

        for (int i = 0; i < nums.size(); i++) {
            diff.push_back((long)i - nums[i]);
        }

        string str = std::to_string(nums[0]);
        for (int i = 1; i < diff.size(); ) {
            if (diff[i] != diff[i - 1]) {
                if (str != "") rst.push_back(str);
                str = std::to_string(nums[i]);
            }
            else {
                while (i < diff.size() - 1 && diff[i] == diff[i + 1]) {
                    i++;
                }
                str += ("->" + std::to_string(nums[i]));
                rst.push_back(str);
                str = "";
            }
            i++;
        }
        if (str != "") rst.push_back(str);
        return rst;
    }
};