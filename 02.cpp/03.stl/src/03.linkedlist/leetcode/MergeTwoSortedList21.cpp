
 
 struct ListNode {
      int val;
      ListNode *next;
      ListNode() : val(0), next(nullptr) {}
      ListNode(int x) : val(x), next(nullptr) {}
      ListNode(int x, ListNode *next) : val(x), next(next) {}
  };
 

 //trial01 - time limit exceeded

 //The program was checked, and found you were accessing memory beyond the declared variables. I skimmed and didn't see it right away, but that is definitely the problem.


class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* node;
        ListNode* rst = node;
        while (l1 != NULL && l2 != NULL) {
            int l = l1 == NULL ? -101 : l1->val;
            int r = l2 == NULL ? -101 : l2->val;
            if (l < r) {
                ListNode* tmp = new ListNode;
                tmp->val = r;
                node->next = tmp;
                node = tmp;
            }
            else {
                ListNode* tmp = new ListNode;
                tmp->val = l;
                node->next = tmp;
                node = tmp;
            }
        }
        node->next = NULL;
        return rst;
    }
};



//trial02

//메모리 에러에서 벗어남

//ListNode* node; 요롷게 선언하고 쓰려고 했는데, 값 할당하려니까 메모리 부족하다고 나와서 new ListNode로 할당해줌

//또한 포인터를 원래는 그냥 NULL썼었는데 cpp에 nullptr 따로 있는 거 같아 이렇게 씀

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* head = new ListNode;
        ListNode* node = new ListNode;
        head = node;
        while (l1 != nullptr && l2 != nullptr) {
            int l = l1 == nullptr ? -101 : l1->val;
            int r = l2 == nullptr ? -101 : l2->val;
            if (l < r) {
                ListNode* tmp = new ListNode;
                tmp->val = r;
                node->next = tmp;
                node = tmp;
                l2 = l2->next;
            }
            else {
                ListNode* tmp = new ListNode;
                tmp->val = l;
                node->next = tmp;
                node = tmp;
                l1 = l1->next;
            }
        }
        node->next = nullptr;
        return head;
    }
};


//trial03 - 거의 다옴

//Your input
//[1, 2, 4]
//[1, 3, 4]

//Output
//[1, 1, 2, 3, 4]

//Expected
//[1, 1, 2, 3, 4, 4]

//맨 마지막에 왜 4가 안들어가지?

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* head = new ListNode;
        ListNode* node = new ListNode;
        head = node;
        while (l1 != nullptr && l2 != nullptr) {
            int l = l1 == nullptr ? 101 : l1->val;
            int r = l2 == nullptr ? 101 : l2->val;
            if (l < r) {
                ListNode* tmp = new ListNode;
                tmp->val = l;
                node->next = tmp;
                node = tmp;
                l1 = l1->next;
            }
            else {
                ListNode* tmp = new ListNode;
                tmp->val = r;
                node->next = tmp;
                node = tmp;
                l2 = l2->next;
            }
        }
        node->next = nullptr;
        return head->next;
    }
};


//solution1

//Runtime: 12 ms, faster than 36.80 % of C++ online submissions for Merge Two Sorted Lists.
//Memory Usage : 15.1 MB, less than 10.28 % of C++ online submissions for Merge Two Sorted Lists.

//and가 아니고 or써야 됬었음

//아 근데 성능이 좀 구리네?

//l1, l2 맨 앞에 애들부터 차례대로 서로 비교해가며, 더 작은애를 넣는 방법

//원본값을 회손 안한다는 장점 존재

//O(N)



class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* head = new ListNode;
        ListNode* node = new ListNode;
        head = node;
        while (l1 != nullptr || l2 != nullptr) {
            int l = l1 == nullptr ? 101 : l1->val;
            int r = l2 == nullptr ? 101 : l2->val;
            if (l < r) {
                ListNode* tmp = new ListNode;
                tmp->val = l;
                node->next = tmp;
                node = tmp;
                l1 = l1->next;
            }
            else {
                ListNode* tmp = new ListNode;
                tmp->val = r;
                node->next = tmp;
                node = tmp;
                l2 = l2->next;
            }
        }
        node->next = nullptr;
        return head->next;
    }
};



//trial04

//구슬꿰듯, l1돌다가 l2에 더 적은거 있으면 l2돌다가 l1에 더 적은거 있으면 다시 l1돌다가

//이런식으로 동작하는애 만드는 중

//Accepted
//Runtime: 0 ms
//Your input
//[1, 2, 3, 4, 5, 11]
//[6, 7, 8, 9, 10, 12]
//Output
//[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
//Expected
//[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]

//요런건 되는데,

//Input
//[2]
//[1]
//Output
//[2, 1]
//Expected
//[1, 2]

//요런건 또 안됨


class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (l2 == nullptr) {
            return l1;
        }
        if (l1 == nullptr) {
            return l2;
        }

        ListNode* head = new ListNode;
        head->next = l1;
        int r = l2->val;

        while (l1->next != nullptr) {
            ListNode* prevL1;
            ListNode* prevL2;

            while (l1 != nullptr && l1->val <= r) {
                prevL1 = l1;
                l1 = l1->next;
            }
            prevL1->next = l2;
            while (l2 != nullptr && l2->val < l1->val) {
                prevL2 = l2;
                l2 = l2->next;
            }
            r = l2->val;
            prevL2->next = l1;
        }
        l1->next = l2;
        return head->next;
    }
};


//trial05

//Wrong Answer
//Runtime : 0 ms
//Your input
//[6, 7, 8, 9, 10, 12]
//[1, 2, 3, 4, 5, 11]
//Output
//[6, 7, 8, 9, 10, 11, 12]
//Expected
//[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]

//첫단추 꿰는게 문제

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (l2 == nullptr) {
            return l1;
        }
        if (l1 == nullptr) {
            return l2;
        }
        ListNode* nullHead = new ListNode(101);
        ListNode* head = new ListNode;
        head->next = l1;
        int r = l2->val;

        while (l1->next != nullptr) {
            ListNode* prevL1 = nullHead;
            ListNode* prevL2 = nullHead;

            while (l1 != nullptr && l1->val <= r) {
                prevL1 = l1;
                l1 = l1->next;
            }
            if (prevL1->val != 101) prevL1->next = l2;

            while (l2 != nullptr && l2->val < l1->val) {
                prevL2 = l2;
                l2 = l2->next;
            }
            if (l2 != nullptr) r = l2->val;
            if (prevL2->val != 101) prevL2->next = l1;
        }
        l1->next = l2;
        return head->next;
    }
};


//trial06

// [1, 2, 3, 4, 5, 11]
// [6,7,8,9,10,12]

// 얘도 되고

//[6, 7, 8, 9, 10, 12]
//[1, 2, 3, 4, 5, 11]

//심지어 얘도 되는데

//Last executed input
//[5]
//[1, 2, 4]

//얘가안됨. nullptr에서 데이터 참조했다고 에러남. 근데 분명 조건문에 if != nullptr 적어놨는데?


class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {

        if (l2 == nullptr) {
            return l1;
        }
        if (l1 == nullptr) {
            return l2;
        }
        ListNode* nullHead = new ListNode(101);
        ListNode* head = new ListNode;

        ListNode* l;
        ListNode* r;

        if (l1->val <= l2->val) {
            l = l1;
            r = l2;
        }
        else {
            l = l2;
            r = l1;
        }
        head->next = l;

        while (l->next != nullptr) {
            ListNode* prevL = nullHead;
            ListNode* prevR = nullHead;

            while (l != nullptr && l->val <= r->val) {
                prevL = l;
                l = l->next;
            }
            if (prevL->val != 101) {
                prevL->next = r;
            }
            while (r != nullptr && r->val <= l->val) {
                prevR = r;
                r = r->next;
            }
            if (prevR->val != 101) {
                prevR->next = l;
            }
        }
        l->next = r;
        return head->next;
    }
};


//solution2

//Runtime: 11 ms, faster than 39.55% of C++ online submissions for Merge Two Sorted Lists.
//Memory Usage : 15 MB, less than 17.12 % of C++ online submissions for Merge Two Sorted Lists.


//아 submit 10번해보니

//solution 1의 성능은
//12,11,20,11,16,12,8,11,12,16 나옴

//solution 2의 성능은
//14,12,12,11,8,8,20,8,11,12 나옴


//solution1의 평균치: 12.9ms
//solution2의 평균치:11.6ms 

//1ms줄음 ㅋㅋㅋ ㅏ...

//solution1은 l1,l2 모든 노드 돌면서 서로 비교해서 더 작은놈은 head에 붙이고
//solution2도 l1,l2 모든 노드 돌면서 서로 비교하긴 하는데, 새로운 head에 붙이는게 아니고 원래 있던 l1,l2 사이에 붙이는 식으로 함
//큰 차이는 없음

//위엣것도 O(N)
//이것도 O(N)이라 별 차이 없는 듯
//아예 접근법이 달라야 하나봄

//잘 보면 solution2는 20ms 극단값이 있음. l1갔다가 l2바꿔끼웠다가 다시 l1바꿔끼웠다가 다시 l2바꿔끼웠다가 무한반복 하면 저리되나봄

//근데 비교할 때 매번 r->val, l->val 참조하는게 아니라, 미리 뽑아놓은 값이랑 비교하면 더 빨라질 것 같기는 헌데..

//근데 어짜피 O(N)에서 벗어나지 못함.

//O(N)이 상위 40%라는 얘기는 O(NlogN)정도는 되야 100%찍는다는 말 같은데


class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (l2 == nullptr) {
            return l1;
        }
        if (l1 == nullptr) {
            return l2;
        }
        ListNode* nullHead = new ListNode(101);
        ListNode* head = new ListNode;

        ListNode* l;
        ListNode* r;

        if (l1->val <= l2->val) {
            l = l1;
            r = l2;
        }
        else {
            l = l2;
            r = l1;
        }
        head->next = l;

        while (l != nullptr) {
            ListNode* prevL = nullHead;
            ListNode* prevR = nullHead;

            while (l != nullptr && l->val <= r->val) {
                prevL = l;
                l = l->next;
            }
            if (prevL->val != 101) {
                prevL->next = r;
                if (l == nullptr) return head->next;
            }

            while (r != nullptr && r->val <= l->val) {
                prevR = r;
                r = r->next;
            }
            if (prevR->val != 101) {
                prevR->next = l;
                if (r == nullptr) return head->next;
            }
        }
        l->next = r;
        return head->next;
    }
};




//solution 3

//Runtime: 8 ms, faster than 74.73% of C++ online submissions for Merge Two Sorted Lists.
//Memory Usage : 15.1 MB, less than 10.35 % of C++ online submissions for Merge Two Sorted Lists.

//역시 뭐든 int[]쓰는게 제일 빠른듯
//simple is the best라 이말이야

//아 근데 아직도 100%가 아님

//O(N)이라 그런지 여러번 submit해도 성능상 큰 차이 없음

//결국 O(N log N)을 해야 100%뜰듯.

//왠지 느낌상 merge sort좀 비틀어서 적용하면 하면 될거같은데


//아 근데 cpp에서 int[] container = new int[201];가 안되길래 찾아봤더니
//int container[201]; 쓰라고 해서
//잉? new없이 써?
//일단 썼는데 아니나 다를까 초기화 안해줘서 이상한 값이 들어있었음
//cpp에서는 array초기화를 int container[201] = { 0 }; 이런식으로 한다네
//java에서는 걍 new로 선언해주면 0으로 자동초기화 해줬는데

//이래서 c계열 언어가 신경쓸게 많다는건가?

//cpp에서는 new키워드 잘 안쓴다고 함. 나중에 메모리 수동으로 해제해줘야 해서리...
//굳이 쓸거면 smart pointer라는걸 쓴다고 함



class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        int container[201] = { 0 };

        while (l1 != nullptr) {
            container[l1->val + 100]++;
            l1 = l1->next;
        }
        while (l2 != nullptr) {
            container[l2->val + 100]++;
            l2 = l2->next;
        }

        ListNode* head = new ListNode(-101);
        ListNode* start = head;

        for (int i = 0; i < 201; i++) {
            while (container[i] > 0) {
                ListNode* tmp = new ListNode(i - 100);
                start->next = tmp;
                start = start->next;
                container[i] -= 1;
            }
        }
        start->next = nullptr;
        return head->next;
    }
};


//solution 4

//merge sort 도입

//기냥 무지성으로다가 l1뒤에 l2이은 다음

//merge sort해버림

//Runtime: 0 ms, faster than 100.00% of C++ online submissions for Merge Two Sorted Lists.
//Memory Usage : 14.9 MB, less than 53.64 % of C++ online submissions for Merge Two Sorted Lists.

//이맛이지...



//experiment 1)

//splitInHalf(source, &a, &b); 말고
//splitInHalf(source, a, b); 하면 안되나?

//a도 포인터잖아?
//reference &를 안쓰고 그냥 splitInHalf(source, a, b);를 넘기면,
//포인터를 넘기는 거라도, 원본값 변경을 안하나봄
//call by value처럼 파라미터로 받을 때 해당 포인터의 값을 복사해서 받나 봄

//그래서 & reference를 써서 원본값 변경을 하나 봄

//자바에서는 객체 넘기면 원본값 변경 가능했었는데 cpp에서는 객체 넘길때도 &로 명시해야 call by reference 되나보네?


//experiment2에 나오지만, 알고보니 splitInHalf()하기 전에 a,b를 걍 초기화만 해놓은 상태였기 때문에, 값을 못넘기고 포인터의 주솟값을 넘길 수 밖에 없어서 &a인 reference로 넘긴 것.



//experiment 2)

//그렇다면, *source = concatenate(a, b); 는 왜 &a로 안넘기지?

//아

//void mergeSort(ListNode** source) { 
//    ListNode* head = *source; 
//    ListNode* a;
//    ListNode* b;
//
//    if (head == nullptr || head->next == nullptr) return;
//
//    splitInHalf(source, &a, &b);

//splitInHalf()에서 a,b넘기기 전에 a,b는 그냥 초기화만 된 상태
//ListNode* a;

//그래서 포인터의 주솟값만 있고 가르키는 값이 없는 상태
//그 상태에서 splitInHalf()에 기냥 a,b 넣어버리면, 값을 넣어버리는게 되는 것이므로 에러나는 것
//근데 splitInHalf()에서 a,b에 값을 할당하고 난 이후엔,
// *source = concatenate(a, b); 에서 a,b 넣어도 값이 있으니 에러 안나는 것


//experiment 3)

//splitInHalf()에서

//ListNode* head = *source;
//ListNode* slow = head;
//ListNode* fast = slow->next;

//얘는 에러걸리는데

// ListNode* fast;
// ListNode* slow;
// slow = *source;
// fast = slow->next;

//얘는 에러 안걸림

//?????


//아
//ListNode* head = *source;
//ListNode* slow = head;

//이렇게 하면, slow가 *source를 가르키는게 아니고, head를 가르켜서 그런가?


//ListNode** head = source;
//ListNode* slow = *head;
//ListNode* fast = slow->next;

//이렇게 하니까 되는걸 보면, 맞는 듯
//head가 source의 값을 가르키게 하지 말고, source의 주솟값을 가르키게 하고,
//*head를 해서 source의 값을 가르키게 하는 방식으로 해야하나 보다.


//그리고 source가 있으니 따로 head 만들 필요도 없음



//experiment 4)

//선언 방식도

// ListNode* fast;
// ListNode* slow;
// slow = *source;
// fast = slow->next;

//이렇게 초기화 한 다음 값을 할당하는게,

//ListNode** head = source;
//ListNode* slow = *head;
//ListNode* fast = slow->next;

//선언하면서 값을 할당하는 것 보다 더 깔끔하게 보임



//experiment 5)

//concatenate()의 재귀적 처리 방식이 어떤식으로 흘러가는거지?

//1. mergeSort()로 입장
//2. splitInHalf()로 반틈 쪼개서 왼쪽 a, 오른쪽 b 부여
//3. 다시 mergeSort()에 반틈 쪼갠 왼쪽 a만 넣음
//4. 원본을 a|b 이렇게 쪼갠 다음, a를 넣었잖아? 또 반으로 쪼갬. a|b | b 이렇게. 그리고 또 a를 mergeSort()에 넣어서 또 반으로 쪼갬.( a|b | b ) | b 이런식으로 a가 null이 될 때까지 무한반복
//5. a가 더이상 쪼갤 수 없는 null이 되면, 이전단계로 복귀함. a | null 이면 a를 return하고 그 윗단계를 감.  a | b이면, 둘 중 작은거를 앞에넣어 붙이고 윗단계로 return 시킴. 
//6. 이렇게 위로 보내기를 하다가, 만약에 a|b가 (2,4),(1,3) 이라면? 맨 처음 a,b에 맨 앞자리만 보고 비교함. 더 작은 1을 붙이고, concatenate()를 재귀함. 1-> ? 인 상태에서 (2,4),(3)에서 앞자리만 비교해서 더 작은걸 붙임.
//   1->2 (4),(3)에서 또 concatenate()를 재귀함 이런식으로 정렬하는 것




//experiment 6)

//그래서 시간복잡도는?

//merge sort는 O(n log n)이라는데, 왜지?

//log의 밑이 2라는 가정하에, 2^? = n이 되기 위해서, log(N)번 쪼개야함. 이게 depth

//그리고 쪼갠 애를 서로 비교해서 붙이는걸 n번 참조한다고 퉁치는 듯?

class Solution {

public: 
    void splitInHalf(ListNode** source, ListNode** a, ListNode** b){

        ListNode* fast;
        ListNode* slow;
        slow = *source;
        fast = slow->next;
            

        while(fast != nullptr){
            fast = fast->next;
            if(fast != nullptr){
                fast = fast->next;
                slow = slow->next;
            }
        }
        
        *a = *source;
        *b = slow->next;
        slow->next = nullptr;
    }
    
    ListNode* concatenate(ListNode* a, ListNode* b){
        if(a == nullptr) return b; 
        if(b == nullptr) return a;
        
        ListNode* tmp = nullptr;
        
        if(a->val < b->val){
            tmp = a;
            tmp->next = concatenate(a->next, b);
        } else {
            tmp = b;
            tmp->next = concatenate(a, b->next);
        }
        return tmp;
    }
    
    void mergeSort(ListNode** source){ 
        ListNode* head = *source; 
        ListNode* a;
        ListNode* b;
        
        if(head == nullptr || head->next == nullptr) return;
        
        splitInHalf(source, &a, &b); 
        
        mergeSort(&a); 
        mergeSort(&b);
        
        *source = concatenate(a, b); 
    }
    
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if(l1 == nullptr) return l2;
        if(l2 == nullptr) return l1;
        ListNode* head = l1;
        
        while(l1->next != nullptr){
            l1 = l1->next;
        }
        l1->next = l2;
        
        mergeSort(&head);
        
        return head;
    }
};


//solution5 by StefanPochmann



//The first line ensures that a is at least as good a list head as b, by swapping them if that's not already the case. 
//The second line merges the remaining lists behind a.



//1)
 
//일단 cpp은 swap()이 기본적으로 구현됬다는점이 놀라움.

//이래서 알고리즘 각잡고 할거면 cpp로 하라는 이유인가?

//2) 

//첫번째 if문에서, 왜 !l1해줘야 하지?

//애초에 첫번째 if문의 존재 이유가, l1이랑 l2중에서 가장 첫번째 element가 더 작은개를 l1으로 두기 위함 아닌가?

//-> !l1하고 or을 쓰기 때문에, 만약에 l1이 nullptr라면, l2를 l1에 넣어줌
 
//그리고 !l1 || 을 해 둬야, 조건문 끝부분에 l1->val에서 메모리 참조 에러 안뜸

//3)

//if(l1 && l2 && (l2->val < l1->val))이 아닌건, l1,l2중 l1이 nullptr이고 l2가 값이 있을 때, swap()해주지 않는구나

//4)

//!l1 && ()이 아닌것도 동일한 이유 때문


class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (!l1 || (l2 && l2->val < l1->val)) swap(l1, l2);
        if (l1) l1->next = mergeTwoLists(l1->next, l2);
        return l1;
    }
};