
 
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

//�޸� �������� ���

//ListNode* node; ���� �����ϰ� ������ �ߴµ�, �� �Ҵ��Ϸ��ϱ� �޸� �����ϴٰ� ���ͼ� new ListNode�� �Ҵ�����

//���� �����͸� ������ �׳� NULL����µ� cpp�� nullptr ���� �ִ� �� ���� �̷��� ��

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


//trial03 - ���� �ٿ�

//Your input
//[1, 2, 4]
//[1, 3, 4]

//Output
//[1, 1, 2, 3, 4]

//Expected
//[1, 1, 2, 3, 4, 4]

//�� �������� �� 4�� �ȵ���?

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

//and�� �ƴϰ� or��� �����

//�� �ٵ� ������ �� ������?

//l1, l2 �� �տ� �ֵ���� ���ʴ�� ���� ���ذ���, �� �����ָ� �ִ� ���

//�������� ȸ�� ���Ѵٴ� ���� ����

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

//�������, l1���ٰ� l2�� �� ������ ������ l2���ٰ� l1�� �� ������ ������ �ٽ� l1���ٰ�

//�̷������� �����ϴ¾� ����� ��

//Accepted
//Runtime: 0 ms
//Your input
//[1, 2, 3, 4, 5, 11]
//[6, 7, 8, 9, 10, 12]
//Output
//[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
//Expected
//[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]

//�䷱�� �Ǵµ�,

//Input
//[2]
//[1]
//Output
//[2, 1]
//Expected
//[1, 2]

//�䷱�� �� �ȵ�


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

//ù���� ��°� ����

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

// �굵 �ǰ�

//[6, 7, 8, 9, 10, 12]
//[1, 2, 3, 4, 5, 11]

//������ �굵 �Ǵµ�

//Last executed input
//[5]
//[1, 2, 4]

//�갡�ȵ�. nullptr���� ������ �����ߴٰ� ������. �ٵ� �и� ���ǹ��� if != nullptr ������µ�?


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


//�� submit 10���غ���

//solution 1�� ������
//12,11,20,11,16,12,8,11,12,16 ����

//solution 2�� ������
//14,12,12,11,8,8,20,8,11,12 ����


//solution1�� ���ġ: 12.9ms
//solution2�� ���ġ:11.6ms 

//1ms���� ������ ��...

//solution1�� l1,l2 ��� ��� ���鼭 ���� ���ؼ� �� �������� head�� ���̰�
//solution2�� l1,l2 ��� ��� ���鼭 ���� ���ϱ� �ϴµ�, ���ο� head�� ���̴°� �ƴϰ� ���� �ִ� l1,l2 ���̿� ���̴� ������ ��
//ū ���̴� ����

//�����͵� O(N)
//�̰͵� O(N)�̶� �� ���� ���� ��
//�ƿ� ���ٹ��� �޶�� �ϳ���

//�� ���� solution2�� 20ms �شܰ��� ����. l1���ٰ� l2�ٲ㳢���ٰ� �ٽ� l1�ٲ㳢���ٰ� �ٽ� l2�ٲ㳢���ٰ� ���ѹݺ� �ϸ� �����ǳ���

//�ٵ� ���� �� �Ź� r->val, l->val �����ϴ°� �ƴ϶�, �̸� �̾Ƴ��� ���̶� ���ϸ� �� ������ �� ����� �嵥..

//�ٵ� ��¥�� O(N)���� ����� ����.

//O(N)�� ���� 40%��� ���� O(NlogN)������ �Ǿ� 100%��´ٴ� �� ������


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

//���� ���� int[]���°� ���� ������
//simple is the best�� �̸��̾�

//�� �ٵ� ������ 100%�� �ƴ�

//O(N)�̶� �׷��� ������ submit�ص� ���ɻ� ū ���� ����

//�ᱹ O(N log N)�� �ؾ� 100%���.

//���� ������ merge sort�� ��Ʋ� �����ϸ� �ϸ� �ɰŰ�����


//�� �ٵ� cpp���� int[] container = new int[201];�� �ȵǱ淡 ã�ƺô���
//int container[201]; ����� �ؼ�
//��? new���� ��?
//�ϴ� ��µ� �ƴϳ� �ٸ��� �ʱ�ȭ �����༭ �̻��� ���� ����־���
//cpp������ array�ʱ�ȭ�� int container[201] = { 0 }; �̷������� �Ѵٳ�
//java������ �� new�� �������ָ� 0���� �ڵ��ʱ�ȭ ����µ�

//�̷��� c�迭 �� �Ű澵�� ���ٴ°ǰ�?

//cpp������ newŰ���� �� �Ⱦ��ٰ� ��. ���߿� �޸� �������� ��������� �ؼ���...
//���� ���Ÿ� smart pointer��°� ���ٰ� ��



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

//merge sort ����

//��� ���������δٰ� l1�ڿ� l2���� ����

//merge sort�ع���

//Runtime: 0 ms, faster than 100.00% of C++ online submissions for Merge Two Sorted Lists.
//Memory Usage : 14.9 MB, less than 53.64 % of C++ online submissions for Merge Two Sorted Lists.

//�̸�����...



//experiment 1)

//splitInHalf(source, &a, &b); ����
//splitInHalf(source, a, b); �ϸ� �ȵǳ�?

//a�� �������ݾ�?
//reference &�� �Ⱦ��� �׳� splitInHalf(source, a, b);�� �ѱ��,
//�����͸� �ѱ�� �Ŷ�, ������ ������ ���ϳ���
//call by valueó�� �Ķ���ͷ� ���� �� �ش� �������� ���� �����ؼ� �޳� ��

//�׷��� & reference�� �Ἥ ������ ������ �ϳ� ��

//�ڹٿ����� ��ü �ѱ�� ������ ���� �����߾��µ� cpp������ ��ü �ѱ涧�� &�� ����ؾ� call by reference �ǳ�����?


//experiment2�� ��������, �˰��� splitInHalf()�ϱ� ���� a,b�� �� �ʱ�ȭ�� �س��� ���¿��� ������, ���� ���ѱ�� �������� �ּڰ��� �ѱ� �� �ۿ� ��� &a�� reference�� �ѱ� ��.



//experiment 2)

//�׷��ٸ�, *source = concatenate(a, b); �� �� &a�� �ȳѱ���?

//��

//void mergeSort(ListNode** source) { 
//    ListNode* head = *source; 
//    ListNode* a;
//    ListNode* b;
//
//    if (head == nullptr || head->next == nullptr) return;
//
//    splitInHalf(source, &a, &b);

//splitInHalf()���� a,b�ѱ�� ���� a,b�� �׳� �ʱ�ȭ�� �� ����
//ListNode* a;

//�׷��� �������� �ּڰ��� �ְ� ����Ű�� ���� ���� ����
//�� ���¿��� splitInHalf()�� ��� a,b �־������, ���� �־�����°� �Ǵ� ���̹Ƿ� �������� ��
//�ٵ� splitInHalf()���� a,b�� ���� �Ҵ��ϰ� �� ���Ŀ�,
// *source = concatenate(a, b); ���� a,b �־ ���� ������ ���� �ȳ��� ��


//experiment 3)

//splitInHalf()����

//ListNode* head = *source;
//ListNode* slow = head;
//ListNode* fast = slow->next;

//��� �����ɸ��µ�

// ListNode* fast;
// ListNode* slow;
// slow = *source;
// fast = slow->next;

//��� ���� �Ȱɸ�

//?????


//��
//ListNode* head = *source;
//ListNode* slow = head;

//�̷��� �ϸ�, slow�� *source�� ����Ű�°� �ƴϰ�, head�� �����Ѽ� �׷���?


//ListNode** head = source;
//ListNode* slow = *head;
//ListNode* fast = slow->next;

//�̷��� �ϴϱ� �Ǵ°� ����, �´� ��
//head�� source�� ���� ����Ű�� ���� ����, source�� �ּڰ��� ����Ű�� �ϰ�,
//*head�� �ؼ� source�� ���� ����Ű�� �ϴ� ������� �ؾ��ϳ� ����.


//�׸��� source�� ������ ���� head ���� �ʿ䵵 ����



//experiment 4)

//���� ��ĵ�

// ListNode* fast;
// ListNode* slow;
// slow = *source;
// fast = slow->next;

//�̷��� �ʱ�ȭ �� ���� ���� �Ҵ��ϴ°�,

//ListNode** head = source;
//ListNode* slow = *head;
//ListNode* fast = slow->next;

//�����ϸ鼭 ���� �Ҵ��ϴ� �� ���� �� ����ϰ� ����



//experiment 5)

//concatenate()�� ����� ó�� ����� ������� �귯���°���?

//1. mergeSort()�� ����
//2. splitInHalf()�� ��ƴ �ɰ��� ���� a, ������ b �ο�
//3. �ٽ� mergeSort()�� ��ƴ �ɰ� ���� a�� ����
//4. ������ a|b �̷��� �ɰ� ����, a�� �־��ݾ�? �� ������ �ɰ�. a|b | b �̷���. �׸��� �� a�� mergeSort()�� �־ �� ������ �ɰ�.( a|b | b ) | b �̷������� a�� null�� �� ������ ���ѹݺ�
//5. a�� ���̻� �ɰ� �� ���� null�� �Ǹ�, �����ܰ�� ������. a | null �̸� a�� return�ϰ� �� ���ܰ踦 ��.  a | b�̸�, �� �� �����Ÿ� �տ��־� ���̰� ���ܰ�� return ��Ŵ. 
//6. �̷��� ���� �����⸦ �ϴٰ�, ���࿡ a|b�� (2,4),(1,3) �̶��? �� ó�� a,b�� �� ���ڸ��� ���� ����. �� ���� 1�� ���̰�, concatenate()�� �����. 1-> ? �� ���¿��� (2,4),(3)���� ���ڸ��� ���ؼ� �� ������ ����.
//   1->2 (4),(3)���� �� concatenate()�� ����� �̷������� �����ϴ� ��




//experiment 6)

//�׷��� �ð����⵵��?

//merge sort�� O(n log n)�̶�µ�, ����?

//log�� ���� 2��� �����Ͽ�, 2^? = n�� �Ǳ� ���ؼ�, log(N)�� �ɰ�����. �̰� depth

//�׸��� �ɰ� �ָ� ���� ���ؼ� ���̴°� n�� �����Ѵٰ� ��ġ�� ��?

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
 
//�ϴ� cpp�� swap()�� �⺻������ ������ٴ����� ����.

//�̷��� �˰��� ����� �ҰŸ� cpp�� �϶�� �����ΰ�?

//2) 

//ù��° if������, �� !l1����� ����?

//���ʿ� ù��° if���� ���� ������, l1�̶� l2�߿��� ���� ù��° element�� �� �������� l1���� �α� ���� �ƴѰ�?

//-> !l1�ϰ� or�� ���� ������, ���࿡ l1�� nullptr���, l2�� l1�� �־���
 
//�׸��� !l1 || �� �� �־�, ���ǹ� ���κп� l1->val���� �޸� ���� ���� �ȶ�

//3)

//if(l1 && l2 && (l2->val < l1->val))�� �ƴѰ�, l1,l2�� l1�� nullptr�̰� l2�� ���� ���� ��, swap()������ �ʴ±���

//4)

//!l1 && ()�� �ƴѰ͵� ������ ���� ����


class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (!l1 || (l2 && l2->val < l1->val)) swap(l1, l2);
        if (l1) l1->next = mergeTwoLists(l1->next, l2);
        return l1;
    }
};