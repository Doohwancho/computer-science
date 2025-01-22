
//생각

//array로 구현하면 될지 싶은데, getMin()에서 걸리네

//.push()할 때마 제일 작은수 기억해놔도, pop()한다음 .getMin()하면 말짱 꽝이잖아?

//그러면 젤 작은수를 10개정도 기억해 두면 되지 않을까? -> 만약 array size가 백만개인데, pop()을 백만번-1 한 다음 getMin()할 수도 있잖아?

//그렇다고 getMin()할 때마다 전체 array를 for loop돌자니 매우 비효율 같은데 어쩌지?



//근데 이거 구현하는데 STack써도 되려나? 그냥 Stack구현하는거면 Stack쓰면 안되는건 상식인데, 이건 MinSTack구현하는거니까 되지 않을까?





//Trial01 - Time Limit Exceeded

//스택 두개 준비해서 .push(n)할 때 n이 a.top()보다 작으면 계속 쌓다가,
//n이 더 큰게 나오면, a를 b쪽에 옮겨쌓는데, 쌓는 도중, n과 비교해서, n을 꼽사리 끼워넣음
//b에 다 쌓으면, b는 젤 큰게 위에있는 식이니까, 다시 a로 옮기면서 뒤집어줌



//["MinStack", "push", "push", "push", "getMin", "pop", "top", "getMin"]
//[[], [-2], [0], [-3], [], [], [], []]

//얘는 pass하는데

//["MinStack", "push", "push", "push", "getMin", "pop", "top", "getMin"]
//[[], [-2], [0], [-3], [], [], [], []]

//얘는 pass 안됨

//하긴 push pop할때마다 이리옮겼다 저리옮겼다하는건 좀 에바지



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

//아 a->b는 문제 없는데, b->a할 때 문제가 생기네?

//a,b를 둘다 queue써볼까? 아 그러면 a.push()할 때 또 문제가 생기네

//답의 윗부분 보니까 stack 두개로 해결 가능하다고 함. 그리고 대충 보니까 졸라 단순해 보였음

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

//처음에 보고 잉? 했던게, 난 stack a에 있는 모든 요소들을 오름차순이건 내림차순이건 정렬해야 한다고 생각했는데,

//얘는 첫빠따보다 큰 애들은 버려버림. 왜냐면 만약 push(1), push(2), push(3) 해서 stack b에 1만 들어갔다고 해도, 최악의 경우 stack a에 pop(), pop() 해봤자, getMin()의 1은 남아있잖아

//모든 요소를 정렬해야 한다는 생각이 오판이었음.


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

//아

//5,5,5,4,3,2,1 이런 순으로 push 했다고 하면

//getMin도 5,5,5,4,3,2,1 이겠네. 그러면 맨 뒤에 1부터 뺀다고 해도 상관 없네

//만약 5,-1,5,4,3,2,1 이렇게 넣는다고 하면,

//min은 5,-1,-1,-1,-1,-1,-1 이렇게 들어가서 상관 없겠네

//stack에 min정보가 같이 담긴 오브젝트를 넣어주는 것

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


