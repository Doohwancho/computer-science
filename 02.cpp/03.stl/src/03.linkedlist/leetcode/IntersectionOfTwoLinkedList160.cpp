#include <iostream>
#include <map>
#include <hash_map>
#include <list>

using namespace std;


struct ListNode {
	int val;
	ListNode* next;
	ListNode(int x) : val(x), next(NULL) {}
};


//���� ���� ������ ó������ loop���鼭 set�� ��� !set.add() �ϴ°� return�ϴ� ������ �ϸ�, A�� B���� �� �����Ѽ��ؼ� �ȵ��ݾ�?

//O(N^2), O(N*M)���� �������� ��� ����



//trial01

//�ʹ� ��ȿ�� + sudo code

class Solution {
public:
    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
        hash_map<int, list<ListNode*>> map;
        ListNode* A = headA;
        ListNode* B = headB;
        ListNode* rst = new ListNode(0);

        while (A != nullptr && B != nullptr) {
            if (A != nullptr) {
                if (map.find(A->val)) {
                    if (map.find(A->val).contains(A) {
                        return new ListNode(A->val);
                    }
                    map.find(A->val).add(A);
                }
                else {
                    map.insert(make_pair(A->val), A);
                }
                A = A->next;
            }
            if (B != nullptr) {
                if (map.find(B->val)) {
                    if (map.find(B->val).contains(B) {
                        return new ListNode(B->val);
                    }
                    map.find(B->val).add(B);
                }
                else {
                    map.insert(make_pair(B->val), B);
                }
                B = B->next;
            }
        }

        return rst;
    }
};


//solution1


//Runtime: 68 ms, faster than 28.37% of C++ online submissions for Intersection of Two Linked Lists.
//Memory Usage : 21.3 MB, less than 5.30 % of C++ online submissions for Intersection of Two Linked Lists.

//time complexity: O(N+M)
//space complexity: O(N)


//cpp���� map.put()���ϰ� map[A] = B; �̷������� �ִ±���


//trial01�� �� �� ������ ������� �ٲ� ��. �ٵ� ���� ������ ����

//A�� �� ���� ���� ã�� ��� ��������?


class Solution {
public:
    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
        map<ListNode*, int> map;
        ListNode* A = headA;
        ListNode* B = headB;

        while (A) {
            map[A]++;
            A = A->next;
        }
        while (B) {
            if (map[B] > 0) {
                return new ListNode(B->val);
            }
            B = B->next;
        }

        return nullptr;
    }
};


//solution 2 by myfavcat

//Runtime: 36 ms, faster than 97.46 % of C++ online submissions for Intersection of Two Linked Lists.
//Memory Usage : 14.4 MB, less than 88.93 % of C++ online submissions for Intersection of Two Linked Lists.

//???

//1. �䳢�� �ź��� �˰��� ������ �� ���� �ѵ�, �̰� �� solution1���� ������ �ι� �� ����? headA�� headB�� �Ѳ����� ��߳��� ������ �̳� �� ���� ���ݾ�?
// solution1��headA�� ��~�� ���� ���� �̻� �׷����� �������Ű�����

//a,b�� ���� ���̸�, 1 traversal�ȿ� ����. ���� ������, �ȸ����� nullptr�� ������,
//a,b�� �ٸ� ���̸�, ���� swap�� �� sync�� �¾Ƽ� 2 traversal�ȿ� ����. 

//�� a,b���̰� 1��ŭ �ٸ��� 10000��ŭ �ٸ���, ���� swap�ϸ� �� ��ũ�� �³�?

//a�� ���̰� 3�̰�, b�� ���̰� 100�̶�� ����

//a�� 3 �����̰� null���� ��, b�� 3��ŭ ���� 97��.

//b�� 97��ŭ �� ������ �� ����, a�� b�� ����������� 97��ŭ �� ������

//�׷��� b�� null��� a�� ������� ����, b�� ���ϼ����� ��

//a+b = b+a �̱� ����

//�� ¾��


class Solution {
public:
    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
        if (headA == nullptr || headB == nullptr) return nullptr;

        ListNode* a = headA;
        ListNode* b = headB;

        while (a != b) {
            a = a == nullptr ? headB : a->next;
            b = b == nullptr ? headA : b->next;
        }

        return a;
    }
};



//solution 3

//a����, b���� ��
//�� ���̸� diff��� �� ��,
//a�� b���� �� ���, diff��ŭ a�� a->next�� �̵���Ŵ
//�׶����� a == b ����

//�̰͵� �״�... 
//solution1���� �� ������. 
//solution1�̶� ���� O(N+M)�̱� �ѵ�, ���� �����ȹ ���, O(2N+2M)�̶� ��� ǥ������� ������ ������ �� ������