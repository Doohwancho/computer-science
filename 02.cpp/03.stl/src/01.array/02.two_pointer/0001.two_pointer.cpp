#include <vector>
#include <algorithm> //for std::find_if
#include <map>

//solution1) 2 pointers
//O(N^2) time, O(1) space
//Runtime: 48 ms, faster than 42.12% of C++ online submissions for Two Sum.
//Memory Usage: 12.7 MB, less than 54.64% of C++ online submissions for Two Sum.
// std::vector<int> twoSum(std::vector<int>& nums, int target) {
//     std::vector<int> result;
//     for(int i = 0; i < (int)nums.size(); i++){
//         for(int j = i+1; j < (int)nums.size(); j++) {
//             if(nums[i] + nums[j] == target) {
//                 result = {i, j};
//                 return result;
//             }
//         }
//     }    
//     return result;
// };

//solution2) 2 pointers with iterator
//O(N) time, O(1) space
//Runtime: 42 ms, faster than 48.10% of C++ online submissions for Two Sum.
//Memory Usage: 12.7 MB, less than 73.86% of C++ online submissions for Two Sum
// std::vector<int> twoSum(std::vector<int>& nums, int target) {
//     std::vector<int> result;
//     for(int i = 0; i < (int)nums.size(); i++){
//         auto it = std::find(nums.begin() + i + 1, nums.end(), target - nums[i]);

//         if(it != nums.end()) {
//             result = {i, static_cast<int>(std::distance(nums.begin(), it))};
//             return result;
//         }
//     }    
//     return result;
// };

//solution3) map
//O(N) time, O(N) space
//Runtime: 7 ms, faster than 86.40% of C++ online submissions for Two Sum.
//Memory Usage: 13.6 MB, less than 47.60% of C++ online submissions for Two Sum.
std::vector<int> twoSum(std::vector<int>& nums, int target) {
    std::map<int, int> map;
    std::vector<int> result;

    for(int i = 0; i < (int)nums.size(); i++){
        int complement = target - nums[i];
        if(map.find(complement)!= map.end()){
            result.push_back(map[complement]);
            result.push_back(i);
            break;
        }
        map[nums[i]] = i;
    }    
    return result;
};