#include <vector>
#include <iostream>

using namespace std;


/*
Java

Runtime: 2 ms, faster than 38.15% of Java online submissions for Shuffle String.
Memory Usage: 41.9 MB, less than 5.49% of Java online submissions for Shuffle String.

    public String restoreString(String s, int[] indices) {
        char[] c = new char[s.length()];
        for(int i = 0; i < s.length(); i++){
            c[indices[i]] = s.charAt(i);
        }
        return new String(c);
    }

*/


/*

//trial01 

dynamic-stack-buffer-overflow 에러남
아마 char c[size]가 컴파일 타임에 만들어지는데 constant말고 s.size()넣어줘서 그런가 봄
char[]말고 vector 써야 겠다

public:
    string restoreString(string s, vector<int>& indices) {
        int size = s.length();
        char c[size];
        for(int i = 0; i < s.length(); i++){
            c[indices[i]] = s.at(i);
        }
        string str(c);

        return str;
    }
*/


/*
//마음엔 안들지만...일단 시간없으니 패스

//Runtime: 10 ms, faster than 39.04 % of C++ online submissions for Shuffle String.
//Memory Usage : 15.2 MB, less than 76.58 % of C++ online submissions for Shuffle String.

class Solution {
public:
    string restoreString(string s, vector<int>& indices) {
        vector<char>v(s.size());
        for (int i = 0; i < s.size(); i++) {
            v[indices[i]] = s[i];
        }
        string ans = "";
        for (int i = 0; i < v.size(); i++) {
            ans += v[i];
        }
        return ans;
    }
};

*/


/*
//swap방식

//이맛이구만 기래


// Runtime: 10 ms, faster than 38.68% of C++ online submissions for Shuffle String.
// Memory Usage : 15.3 MB, less than 42.39 % of C++ online submissions for Shuffle String.


class Solution {
public:
    string restoreString(string s, vector<int>& indices) {
        char tmpc;
        int tmpi;

        for (int i = 0; i < s.length(); ) {
            if (i == indices[i]) {
                i++;
                continue;
            }

            tmpc = s.at(indices[i]);
            tmpi = indices[indices[i]];

            s[indices[i]] = s[i];
            s[i] = tmpc;

            indices[indices[i]] = indices[i];
            indices[i] = tmpi;
        }
        return s;
    }
};

*/


//swap ver.2

//Runtime: 8 ms, faster than 72.22% of C++ online submissions for Shuffle String.
//Memory Usage : 15.2 MB, less than 42.39 % of C++ online submissions for Shuffle String.

//좀더 군더더기가 없는 swap
//와 이거지


//codeceet codeleet
//codecoet codeleet
//codecodt codeleet
//codecode codeleet
//lodecode codeleet
//loeecode codeleet
//leeecode codeleet
//leetcode codeleet

//cpp에서는 걍 string p = s; 냅다 박아도 deepcopy가 되네?


class Solution {
public:
    string restoreString(string s, vector<int>& indices) {
        string p = s;
        char b;
        int a;
        for (int i = 0; i < indices.size(); i++) {
            a = indices[i];
            b = s[i];
            p[a] = b;
        }
        return p;
    }
};





//Runtime: 16 ms, faster than 14.53% of C++ online submissions for Shuffle String.
//Memory Usage : 15.3 MB, less than 42.39 % of C++ online submissions for Shuffle String.

//swap보다 더 빠를 줄 알았는데 char vector -> string이 느린가 봄



class Solution {
public:
    string restoreString(string s, vector<int>& indices) {
        vector<char> v(s.length());

        for (int i = 0; i < s.length(); i++) {
            v[indices[i]] = s.at(i);
        }
        string str(v.begin(), v.end());
        return str;
    }
};


//얘랑 위에거랑 같긴 한데 한줄 더 짧네

class Solution {
public:
    string restoreString(string s, vector<int>& indices) {
        string r(s.length(), ' ');
        for (int i = 0; i < indices.size(); i++) {
            r[indices[i]] = s[i];
        }
        return r;
    }
};