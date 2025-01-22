//trial01 - time limit exceeded

//너무 무지성이다

class KthLargest {

    vector<int> v;
    int idx;

public:
    KthLargest(int k, vector<int>& nums) {
        v = nums;
        idx = k - 1;
    }

    int add(int val) {
        v.push_back(val);
        sort(v.begin(), v.end(), greater <>());
        return v.size() >= idx + 1 ? v.at(idx) : v.at(v.size() - 1);
    }
};




//solution1

//Runtime: 1796 ms, faster than 5.12% of C++ online submissions for Kth Largest Element in a Stream.
//Memory Usage : 20.8 MB, less than 10.66 % of C++ online submissions for Kth Largest Element in a Stream.

//되긴 되는데 조올~~~라 느림

//add()할 때마다 k번째까지 loop하는 것 때문에 그런 듯.

//linked list를 k번째까지 제한하고,

//마지막 노드만 따서 return node 이런식으로 해야 성능 좋아질 듯


class KthLargest {

    struct Node {
        int data;
        Node* next;
    };

    Node* list = NULL;
    int kth;

public:

    // Add - add ascending order
    void descending_order_add(int key) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->data = key;
        new_node->next = NULL;

        if (list == NULL) {
            list = new_node;
        }
        else {
            Node* cur = list;
            Node* prev = NULL;

            // If first element is smaller than key
            if (cur->data <= new_node->data) {
                new_node->next = cur;
                list = new_node;
            }
            // Other cases
            else {
                while (cur != NULL && cur->data > new_node->data) {
                    prev = cur;
                    cur = cur->next;
                }
                // Add in middle
                if (cur != NULL) {
                    new_node->next = cur;
                    prev->next = new_node;
                }
                // Add to end
                else {
                    prev->next = new_node;
                }
            }
        }
    }

    KthLargest(int k, vector<int>& nums) {
        kth = k - 1;
        for (int i = 0; i < nums.size(); i++) {
            descending_order_add(nums[i]);
        }
    }

    int add(int val) {
        descending_order_add(val);
        int i = kth;
        Node* cur = list;
        while (i > 0 && cur != NULL) {
            cur = cur->next;
            i--;
        }
        return cur->data;
    }
};
