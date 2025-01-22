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


//둘이 붙인 다음에 처음부터 loop돌면서 set에 담고 !set.add() 하는거 return하는 식으로 하면, A랑 B붙일 때 원본훼손해서 안되잖아?

//O(N^2), O(N*M)말고 간지나는 방법 없나



//trial01

//너무 비효율 + sudo code

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


//cpp에선 map.put()안하고 map[A] = B; 이런식으로 넣는구나


//trial01을 좀 더 괜찮은 방법으로 바꾼 것. 근데 아직 성능이 구림

//A를 다 돌기 전에 찾는 방법 없으려나?


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

//1. 토끼와 거북이 알고리즘 응용한 거 같긴 한데, 이게 왜 solution1보다 성능이 두배 더 좋지? headA랑 headB랑 한끗차로 어긋나서 뺑뺑이 겁나 돌 수도 있잖아?
// solution1은headA가 졸~라 길지 않은 이상 그럭저럭 괜찮을거같은데

//a,b가 같은 길이면, 1 traversal안에 끝남. 둘이 만나든, 안만나서 nullptr로 끝나든,
//a,b가 다른 길이면, 둘이 swap할 때 sync가 맞아서 2 traversal안에 끝남. 

//와 a,b길이가 1만큼 다르건 10000만큼 다르건, 둘이 swap하면 딱 싱크가 맞네?

//a의 길이가 3이고, b의 길이가 100이라고 하자

//a가 3 움직이고 null찍을 때, b도 3만큼 가서 97임.

//b가 97만큼 더 움직일 때 까지, a도 b의 출발지점부터 97만큼 더 움직임

//그러다 b가 null찍고 a의 출발지로 오면, b랑 동일선상이 됨

//a+b = b+a 이기 때문

//와 쩐다


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

//a길이, b길이 잼
//그 차이를 diff라고 할 때,
//a가 b보다 더 길면, diff만큼 a를 a->next로 이동시킴
//그때부터 a == b 비교함

//이것도 그닥... 
//solution1보다 더 구릴듯. 
//solution1이랑 같이 O(N+M)이긴 한데, 실제 실행계획 까보면, O(2N+2M)이라서 빅오 표기법상은 같은데 성능은 더 구릴듯