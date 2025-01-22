
//����

//array�� �����ϸ� ���� ������, getMin()���� �ɸ���

//.push()�� ���� ���� ������ ����س���, pop()�Ѵ��� .getMin()�ϸ� ��¯ �����ݾ�?

//�׷��� �� �������� 10������ ����� �θ� ���� ������? -> ���� array size�� �鸸���ε�, pop()�� �鸸��-1 �� ���� getMin()�� ���� ���ݾ�?

//�׷��ٰ� getMin()�� ������ ��ü array�� for loop���ڴ� �ſ� ��ȿ�� ������ ��¼��?



//�ٵ� �̰� �����ϴµ� STack�ᵵ �Ƿ���? �׳� Stack�����ϴ°Ÿ� Stack���� �ȵǴ°� ����ε�, �̰� MinSTack�����ϴ°Ŵϱ� ���� ������?





//Trial01 - Time Limit Exceeded

//���� �ΰ� �غ��ؼ� .push(n)�� �� n�� a.top()���� ������ ��� �״ٰ�,
//n�� �� ū�� ������, a�� b�ʿ� �Űܽ״µ�, �״� ����, n�� ���ؼ�, n�� �Ż縮 ��������
//b�� �� ������, b�� �� ū�� �����ִ� ���̴ϱ�, �ٽ� a�� �ű�鼭 ��������



//["MinStack", "push", "push", "push", "getMin", "pop", "top", "getMin"]
//[[], [-2], [0], [-3], [], [], [], []]

//��� pass�ϴµ�

//["MinStack", "push", "push", "push", "getMin", "pop", "top", "getMin"]
//[[], [-2], [0], [-3], [], [], [], []]

//��� pass �ȵ�

//�ϱ� push pop�Ҷ����� �̸��Ű�� �����Ű���ϴ°� �� ������



class MinStack {

    stack<int> st;
    stack<int> a;
    stack<int> b;

public:
    MinStack() {

    }

    void push(int val) {
        st.push(val);

        if (a.size() == 0 || a.top() >= val) {
            a.push(val);
        }
        else {
            while (a.size() > 0) {
                int aTop = a.top();
                if (aTop > val) {
                    b.push(val);
                    continue;
                }
                b.push(aTop);
                a.pop();
            }
            if (b.top() < val) {
                b.push(val);
            }
            while (b.size() > 0) {
                a.push(b.top());
                b.pop();
            }
        }
    }

    void pop() {
        int target = st.top();
        st.pop();

        while (a.size() > 0) {
            if (a.top() == target) {
                a.pop();
                continue;
            }
            else {
                b.push(a.top());
                a.pop();
            }
        }
        while (b.size() > 0) {
            a.push(b.top());
            b.pop();
        }
    }

    int top() {
        return st.top();
    }

    int getMin() {
        return a.top();
    }
};



//Trial02

//["MinStack", "push", "push", "push", "getMin", "pop", "top", "getMin"]
//[[], [-2], [0], [-3], [], [], [], []]

//�� a->b�� ���� ���µ�, b->a�� �� ������ �����?

//a,b�� �Ѵ� queue�Ẽ��? �� �׷��� a.push()�� �� �� ������ �����

//���� ���κ� ���ϱ� stack �ΰ��� �ذ� �����ϴٰ� ��. �׸��� ���� ���ϱ� ���� �ܼ��� ������

class MinStack {

    stack<int> st;
    stack<int> a;
    queue<int> b;
    bool flag = true;

public:
    MinStack() {

    }

    void push(int val) {
        st.push(val);

        if (flag) {
            if (a.size() == 0 || a.top() >= val) {
                a.push(val);
            }
            else {
                while (a.size() > 0) {
                    int aTop = a.top();
                    if (aTop > val) {
                        b.push(val);
                        continue;
                    }
                    b.push(aTop);
                    a.pop();
                }
                if (b.front() < val) {
                    b.push(val);
                }
                flag = false;
            }
        }
        else {
            if (b.size() == 0 || b.front() >= val) {
                b.push(val);
            }
            else {
                while (b.size() > 0) {
                    int bFront = b.front();
                    if (bFront > val) {
                        a.push(val);
                        continue;
                    }
                    a.push(bFront);
                    b.pop();
                }
                if (a.top() > val) {
                    a.push(val);
                }
                flag = true;
            }
        }
    }

    void pop() {
        int target = st.top();
        st.pop();

        if (flag) {
            while (a.size() > 0) {
                if (a.top() == target) {
                    a.pop();
                    continue;
                }
                else {
                    b.push(a.top());
                    a.pop();
                }
            }
            flag = false;
        }
        else {
            while (b.size() > 0) {
                if (b.front() == target) {
                    b.pop();
                    continue;
                }
                else {
                    a.push(b.front());
                    b.pop();
                }
            }
            flag = true;
        }


    }

    int top() {
        return st.top();
    }

    int getMin() {
        int rst = 0;
        if (flag && a.size() > 0) {
            return a.top();
        }
        else if (!flag && b.size() > 0) {
            return b.front();
        }
        return rst;
    }
};


//solution1 by zjchenRice


//Runtime: 20 ms, faster than 87.17 % of C++ online submissions for Min Stack.
//Memory Usage : 16.4 MB, less than 32.86 % of C++ online submissions for Min Stack.

//ó���� ���� ��? �ߴ���, �� stack a�� �ִ� ��� ��ҵ��� ���������̰� ���������̰� �����ؾ� �Ѵٰ� �����ߴµ�,

//��� ù�������� ū �ֵ��� ��������. �ֳĸ� ���� push(1), push(2), push(3) �ؼ� stack b�� 1�� ���ٰ� �ص�, �־��� ��� stack a�� pop(), pop() �غ���, getMin()�� 1�� �������ݾ�

//��� ��Ҹ� �����ؾ� �Ѵٴ� ������ �����̾���.


class MinStack {

    stack<int> a;
    stack<int> b;


public:
    MinStack() {

    }

    void push(int val) {
        a.push(val);
        if (b.size() == 0 || b.top() >= val) b.push(val);
    }

    void pop() {
        if (a.top() == b.top()) b.pop();
        a.pop();
    }

    int top() {
        return a.top();
    }

    int getMin() {
        return b.top();
    }
};



//solution2 by leftpeter2

//��

//5,5,5,4,3,2,1 �̷� ������ push �ߴٰ� �ϸ�

//getMin�� 5,5,5,4,3,2,1 �̰ڳ�. �׷��� �� �ڿ� 1���� ���ٰ� �ص� ��� ����

//���� 5,-1,5,4,3,2,1 �̷��� �ִ´ٰ� �ϸ�,

//min�� 5,-1,-1,-1,-1,-1,-1 �̷��� ���� ��� ���ڳ�

//stack�� min������ ���� ��� ������Ʈ�� �־��ִ� ��

//O(1)

//Runtime: 16 ms, faster than 97.53 % of C++ online submissions for Min Stack.
//Memory Usage : 16.6 MB, less than 13.16 % of C++ online submissions for Min Stack.


class MinStack {

    stack<pair<int, int>>st;

public:
    void push(int val) {
        if (st.size() == 0) {
            st.push({ val,val });
        }
        else {
            int minimum = st.top().second;
            minimum = min(minimum, val);
            st.push({ val,minimum });
        }
    }

    void pop() {
        if (st.size() == 0) return;
        st.pop();
    }

    int top() {
        return st.top().first;
    }

    int getMin() {
        return st.top().second;
    }
};


