#include <iostream>

using namespace std;

  
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


//solution 1

//Runtime: 4 ms, faster than 98.80 % of C++ online submissions for Remove Duplicates from Sorted List.
//Memory Usage : 11.5 MB, less than 97.31 % of C++ online submissions for Remove Duplicates from Sorted List.

//선형적으로 돌면서

//prev와 curr이 같으면, curr을 지워줘야 하니까,

//curr을 한칸 건너뛰고, prev랑 한칸 건너뛴애를 붙여줌

//문제 난이도:1
 
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (head == nullptr) return head;
        ListNode* prev = head;
        ListNode* curr = prev->next;
        while (curr != nullptr) {
            if (curr->val == prev->val) {
                curr = curr->next;
                prev->next = curr;
            }
            else {
                prev = curr;
                curr = curr->next;
            }
        }
        return head;
    }
};