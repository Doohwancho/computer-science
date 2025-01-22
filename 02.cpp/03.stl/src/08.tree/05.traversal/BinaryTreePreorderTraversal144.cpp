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

//100%�� �ƴѰ� ����, binary search tree�� �̿��Ҹ��� �������� �ֳ���

//�Ѱ��� ��� ���� preOrder(vector<int> v, ...) �ϸ�, 

//pre order traversal ������ vector �� üũ�غ��� �������

//�ֳĸ�, cpp�� object�� �׳� �ѱ�� call by value����, call by reference�� �ѱ�� ������ & ��� �ϳ���




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

//�ٵ� ���۷��� ���� �����ͷ� vector �Ѱܺ���?

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

//�׷��� (*v).push_back(root->val);�� return *v ���� ������. runtime error: reference binding to address 0x~~~ with insufficient space for an object of type const std:vector<int> ...

//�Ƹ� vector<int> v; �̷��� �����ϴ°Ŷ� vector<int>* v; �̷��� �����ͷ� �����ϴ°��� ���̴�

//vector<int> v; �̷��� �����ϸ� ptr1 -> memory space for array �̷��� �����޴µ�(���� push_back()�� ���ߴٰ� �ص� �ʱ�ȭ������ �ϳ���)
//vector<int>* v; �̷��� �����ع�����, ptr2 -> ptr1 �̷��� �����޾Ƽ� memory space for array ���� �ƿ� ���޳� ��

//�׷��ϱ� push_back()�� ��, ���� ���ٰ� �����ߴ°� �ƴұ�?






//Solution1 �� �ٸ� ����

//������ ���� �����ͷ� �ϴ°� �ƴ϶� vector<int> v; �� �Ϲ����̰� �����ϰ�,

//�Ķ���ͷ� �Ѱ��� ��, vector pointer�� �Ѱ���

//vector pointer�� �Ѱ�������, vector<int>* v;�� ����

//vector pointer�� deference�ؾ� �ϴϱ� (*v)ó���� ����

//���⼭ �ű��� ����, v->push_back(root->val);�� �����ٴ°���

//����?

//�� ã�ƺ��� �׳� (*v).push_back(root->val); ��� ���ٰ� ��. syntatic sugar

//�̷� ����� ���°� �� ���� �ʳ� �� ��򸱰Ű�����


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



//�� binary search tree�� �ƴϰ� binary tree��?

//�׷��� ���� �����Ѱ� �������� �ʳ�? �� ���ϰ� �ֳ�?

//insert�� �� Ư���� ��Ģ�� �ִ°� �ƴ��ݾ�? �׷���, �� ��Ģ�� �̿��ؼ� ����ȭ �Ұ� ���ٴ� ���ε�

//vector�� �ƴ϶� �ٸ��ɽ�ߵǳ�?


//Solution2

//by pavel-shlyk

//Runtime: 0 ms, faster than 100.00 % of C++ online submissions for Binary Tree Preorder Traversal.
//Memory Usage : 8.4 MB, less than 44.99 % of C++ online submissions for Binary Tree Preorder Traversal.

//Note that in this solution only right children are stored to stack.

//public List<Integer> preorderTraversal(TreeNode node) {
//    List<Integer> list = new LinkedList<Integer>();
//    Stack<TreeNode> rights = new Stack<TreeNode>();
//    while (node != null) {

//�̱��� ���� ǰ

//pre order traversal�� stack���� ���� ����� �ֱ���

//cpp������ stack.pop()�ϸ� object��ȯ ���ϰ� ���Ÿ� �� �Ŀ� void��ȯ�ϳ�



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