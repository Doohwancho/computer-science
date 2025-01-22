

//solution1

//Runtime: 56 ms, faster than 11.46 % of C++ online submissions for License Key Formatting.
//Memory Usage : 284.1 MB, less than 5.11 % of C++ online submissions for License Key Formatting.

//�ð� ��� ���� �ϱ� �ߴµ�

//���ɵ� ������ �������� �巯�� ��� 

//���ϴ�

//rst += tmp; ��� rst.append(tmp); �ߴµ�, ������ 4ms���� ������

class Solution {
public:
    string licenseKeyFormatting(string s, int k) {
        stack<string> st;
        string str;
        string rst;
        int cnt = k;


        for (int i = s.size() - 1; i >= 0; i--) {
            if (s.at(i) != '-') {
                if (cnt > 0) {
                    char upperedLetter = std::toupper(s.at(i));
                    str = (upperedLetter + str);
                    cnt--;
                }
                else {
                    st.push(str);
                    str = std::toupper(s.at(i));
                    cnt = k - 1;
                }
            }
        }
        if (str.length() > 0) {
            st.push(str);
        }

        while (st.size() > 0) {
            string tmp = st.top();
            tmp.push_back('-');
            rst += tmp;
            st.pop();
        }
        return rst.substr(0, rst.size() - 1);
    }
};


//trial01

//time limit exceeded

//�� solution1�� �ǰ� ��� �ȵ���?

//���� ���� string input�� k=1������ �ִ°ſ��� ����

//�Ƹ� solution1������ ���ÿ� ������ rst�� ���� ��, string.push_back()���µ�, �� �� char node .next�� ���༭ ������ O(N)�ε�

//�ٵ� �갰�� ��쿡,÷���� ������ string�� +������ �ϴϱ�,

//new char + rst �ε�,

//�տ� ���� �� ����, ���� rst�� full scan �ؾ��ϴϱ�, O(N^2)���� �ȵǳ� ��

//��;;;

//�׳� for������ '-'�����ϰ� ���ڸ� uppercase�� ���ڷ� �� ���� ������, �ڿ������� for�� ���鼭 k��°���� '-' insert�ϸ� ���� ������

//�ٵ� �̰� string���� �ϸ�, trial01�� ������ ������ ������ �״ϱ�

//linkedlist�� �ؾ��ϳ�?

//linkedlist�� '-' ���� ����, �ٽ� ÷���� ������ ���鼭 string.push_back() ����� �ϴϱ�, 

//��������δ� O(3N) �̾ O(N)�̰ڳ�?

//�ٵ� �̷��� �غ��� �ᱹ O(2N)���� solution1���� �����ݾ�?

//��...


class Solution {
public:
    string licenseKeyFormatting(string s, int k) {
        string str;
        string rst;
        int cnt = k;

        for (int i = s.size() - 1; i >= 0; i--) {
            if (s.at(i) != '-') {
                if (cnt > 0) {
                    char upperedLetter = std::toupper(s.at(i));
                    str = (upperedLetter + str);
                    cnt--;
                }
                else {
                    rst = rst.length() == 0 ? str : (str + '-' + rst);
                    str = std::toupper(s.at(i));
                    cnt = k - 1;
                }
            }
        }
        if (str.length() > 0) {
            rst = rst.length() == 0 ? str : (str + '-' + rst);
        }

        if (rst.at(0) == '-') {
            rst = rst.substr(1, rst.size());
        }
        return rst;
    }
};



//solution2

//Runtime: 432 ms, faster than 8.22 % of C++ online submissions for License Key Formatting.
//Memory Usage : 8.3 MB, less than 43.43 % of C++ online submissions for License Key Formatting.

//�б� �������µ�, ������ �ȵ�θ޴ٱ����� ������

//rst.insert()�� ������ ������ ��. �Ź� insert�� ������ ���� string+1 ������ŭ ���� ��ֳܳ���.

//cpp���� stringbuilder������ ����?

//-> stringstream�̶��� �ִµ�, ������ �����ٰ� ��


class Solution {
public:
    string licenseKeyFormatting(string s, int k) {
        string rst;

        for (int i = 0; i < s.size(); i++) {
            if (s.at(i) != '-') {
                rst.push_back(std::toupper(s.at(i)));
            }
        }

        int idx = rst.size() - k;

        while (idx > 0) {
            rst.insert(idx, "-");
            idx -= k;
        }

        return rst;
    }
};


//doubly linked list�ε� ���� ������ �� ������?

//s�� ��ٷ� ���鼭 �빮�ڷ� doubly linked list�� �ִµ�, k��°���� '-' �־���

//�� ������, doubly linked list�� ��ȸ�� ���鼭, string rst;�� rst.push_back()����

//push_back()�� �ڿ��� �� ���̴� �ִϱ� ���ɵ� ��������?

//solution3

//Runtime: 20 ms, faster than 14.23 % of C++ online submissions for License Key Formatting.
//Memory Usage : 17.7 MB, less than 5.23 % of C++ online submissions for License Key Formatting.

//O(N)

//56ms -> 20ms

//�� ���� �پ��µ��� ���� ������ �ִ�.



class Solution {

    struct Node {
        char data;
        Node* next, * prev;
        Node() {
            next = prev = NULL;
            data = NULL;
        }
    };

public:
    string licenseKeyFormatting(string s, int k) {
        Node* node = new Node();
        int cnt = k;

        for (int i = s.size() - 1; i >= 0; ) {
            if (s.at(i) != '-') {
                if (cnt > 0) {
                    node->data = std::toupper(s.at(i));
                    node->prev = new Node();
                    Node* tmp = node;
                    node = node->prev;
                    node->next = tmp;
                    cnt--;
                    i--;
                }
                else {
                    node->data = '-';
                    node->prev = new Node();
                    Node* tmp = node;
                    node = node->prev;
                    node->next = tmp;
                    cnt = k;
                }
            }
            else {
                i--;
            }
        }

        node = node->next;

        string rst;

        while (node != NULL) {
            rst.push_back(node->data);
            node = node->next;
        }

        return rst;
    }
};



//s�� ���� �Ȱ��� rst����� ����

//two pointer������ k 1�� ��鼭 s.size() % k�� >0�� ��, s�� ���̰�, 0�Ǹ� '-'���̰� k ���󺹱��ϴ� ������ �ϴ°� ���ϰŰ���

//�� ���� ���� ���ƿ´���

//�� �ٵ� s�� ���Ե� dash ���� �������ݾ�? �׷��� %�ؼ� ������ ���� �ȳ����ڳ�

//�׳� ���� �ܼ��ϰ� �ڿ������� for loop���鼭 k��° ������ �� '-'������ ���ְ�, k��°�� �� '-'�� insert�Ѵٸ�?

//�ٵ� �ƹ��� string.remove()�� ������ ���ٰ� �ص�, string.isert()�� ������ ���̶�°� ������ �ôµ�, �����Ǽ� �ݺ��ϵ�





//solution 4 by yuxiangmusic

//Runtime: 0 ms, faster than 100.00 % of C++ online submissions for License Key Formatting.
//Memory Usage : 8.2 MB, less than 68.77 % of C++ online submissions for License Key Formatting.

//cpp���� Ÿ�� �߷��� ��(int���� char���� ��Ȯ���� ���� ��) auto Ű���� ���ٰ� ��. int�� �ٲ㵵 ������ ��?

//�ؼ� auto->int�� �ٲ���� ������.

//Ȥ�� string�� ������ �ָ� ����Ű�� �ؼ� char�̳� string��ôµ��� �������°� ����, auto Ű����� ������ �ּҰ� ��Ÿ�� �� ���� Ÿ��Ű�����ε�

//solution3�̶� ������ �Ȱ�����, �� �������̰� 20ms�̻󳪴°���?

//s.size() % k�ϴ°� ���, int cnt = k; �� ����, if else�� cnt���ְ�, 0�Ǹ� �ٽ� k�־��ְ� �̷��°Ŷ�, doubly linked list���� ���°Ű� 20ms�� ���̳���?

//�� ����� res.push_back()���ϰ� res += toupper(*i);�Ѱ� ����, cpp������ �ڹٶ� �ٸ��� +=�ص� ������ ��������

//reverse �����Լ��� ������ ���� ���� ���ΰ���.


class Solution {

public:
    string licenseKeyFormatting(string S, int K) {
        string res;
        for (auto i = S.rbegin(); i < S.rend(); i++)
            if (*i != '-') { // ignore '-' in original string
                if (res.size() % (K + 1) == K) res += '-'; // every (K+1)th char is '-' from tail
                res += toupper(*i);
            }

        reverse(res.begin(), res.end());
        return res;
    }
};



