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

//���������� ���鼭

//prev�� curr�� ������, curr�� ������� �ϴϱ�,

//curr�� ��ĭ �ǳʶٰ�, prev�� ��ĭ �ǳʶھָ� �ٿ���

//���� ���̵�:1
 
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