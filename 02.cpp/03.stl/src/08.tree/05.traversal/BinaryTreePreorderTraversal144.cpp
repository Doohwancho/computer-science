#include <iostream>

using namespace std;



struct TreeNode {
	int val;
	TreeNode* left;
	TreeNode* right;
	TreeNode() : val(0), left(nullptr), right(nullptr) {}
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
	TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
	
};

//solution1

//Runtime: 4 ms, faster than 44.88 % of C++ online submissions for Binary Tree Preorder Traversal.
//Memory Usage : 8.3 MB, less than 94.14 % of C++ online submissions for Binary Tree Preorder Traversal.

//100%가 아닌걸 보면, binary search tree를 이용할만한 껀덕지가 있나봄

//한가지 배운 점은 preOrder(vector<int> v, ...) 하면, 

//pre order traversal 끝나고 vector 안 체크해보면 비어있음

//왜냐면, cpp은 object라도 그냥 넘기면 call by value여서, call by reference로 넘기고 싶으면 & 써야 하나봄




class Solution {
public:
    void preOrder(vector<int>& v, TreeNode* root) {
        if (root == nullptr) return;
        v.push_back(root->val);
        preOrder(v, root->left);
        preOrder(v, root->right);
    }

    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> v;
        preOrder(v, root);
        return v;
    }
};


//Trial01

//근데 레퍼런스 말고 포인터로 vector 넘겨볼까?

class Solution {
public:
    void preOrder(vector<int>* v, TreeNode* root) {
        if (root == nullptr) return;
        (*v).push_back(root->val);
        preOrder(v, root->left);
        preOrder(v, root->right);
    }

    vector<int> preorderTraversal(TreeNode* root) {
        vector<int>* v;
        preOrder(v, root);
        return *v;
    }
};

//그러면 (*v).push_back(root->val);랑 return *v 에서 에러남. runtime error: reference binding to address 0x~~~ with insufficient space for an object of type const std:vector<int> ...

//아마 vector<int> v; 이렇게 선언하는거랑 vector<int>* v; 이렇게 포인터로 선언하는것의 차이는

//vector<int> v; 이렇게 선언하면 ptr1 -> memory space for array 이렇게 배정받는데(아직 push_back()을 안했다고 해도 초기화정도는 하나봄)
//vector<int>* v; 이렇게 선언해버리면, ptr2 -> ptr1 이렇게 배정받아서 memory space for array 배정 아예 못받나 봄

//그러니까 push_back()할 때, 공간 없다고 에러뜨는게 아닐까?






//Solution1 의 다른 버전

//선언을 벡터 포인터로 하는게 아니라 vector<int> v; 로 일반적이게 선언하고,

//파라미터로 넘겨줄 때, vector pointer를 넘겨줌

//vector pointer로 넘겨줬으니, vector<int>* v;로 받음

//vector pointer를 deference해야 하니까 (*v)처리를 해줌

//여기서 신기한 점은, v->push_back(root->val);도 먹힌다는거임

//왜지?

//아 찾아보니 그냥 (*v).push_back(root->val); 얘랑 같다고 함. syntatic sugar

//이런 기능은 없는게 더 낫지 않나 더 헤깔릴거같은데


class Solution {
public:
    void preOrder(vector<int>* v, TreeNode* root) {
        if (root == nullptr) return;
        (*v).push_back(root->val); //v->push_back(root->val);
        preOrder(v, root->left);
        preOrder(v, root->right);
    }

    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> v;
        preOrder(&v, root);
        return v;
    }
};



//아 binary search tree가 아니고 binary tree네?

//그러면 위에 구현한게 정석이지 않나? 더 줄일게 있나?

//insert할 때 특정한 규칙이 있는게 아니잖아? 그러면, 그 규칙을 이용해서 최적화 할게 없다는 말인데

//vector가 아니라 다른걸써야되나?


//Solution2

//by pavel-shlyk

//Runtime: 0 ms, faster than 100.00 % of C++ online submissions for Binary Tree Preorder Traversal.
//Memory Usage : 8.4 MB, less than 44.99 % of C++ online submissions for Binary Tree Preorder Traversal.

//Note that in this solution only right children are stored to stack.

//public List<Integer> preorderTraversal(TreeNode node) {
//    List<Integer> list = new LinkedList<Integer>();
//    Stack<TreeNode> rights = new Stack<TreeNode>();
//    while (node != null) {

//이까지 보고 품

//pre order traversal이 stack으로 도는 방법도 있구나

//cpp에서는 stack.pop()하면 object반환 안하고 제거만 한 후에 void반환하네



class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> v;
        stack<TreeNode*> st;

        while (root != nullptr) {

            v.push_back(root->val);

            if (root->right != nullptr) {
                st.push(root->right);
            }

            root = root->left;

            if (root == nullptr && !st.empty()) {
                root = st.top();
                st.pop();
            }
        }
        return v;
    }
};