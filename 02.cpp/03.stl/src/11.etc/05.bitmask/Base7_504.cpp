
//solution1

//Runtime: 0 ms, faster than 100.00 % of C++ online submissions for Base 7.
//Memory Usage : 6.3 MB, less than 9.95 % of C++ online submissions for Base 7.

//O(1)

//���� �������ѵ� 100% ����;;

//������ �������� ���� �Ƹ������� ������...

//�Ƹ���� Ǯ �� �ִ¹� ����


class Solution {
public:
    string convertToBase7(int num) {
        string rst;

        if (num == 0) {
            return "0";
        }

        bool negative = false;

        if (num < 0) {
            negative = true;
            num = abs(num);
        }

        for (int i = 8; i >= 0; i--) {
            int sqrd = pow(7, i);
            if (num >= sqrd) {
                rst += to_string(num / sqrd);
                num %= sqrd;
            }
            else if (rst.length() > 0) {
                rst += "0";
            }
        }

        if (negative) {
            rst.insert(0, "-");
        }

        return rst;
    }
};





//solution2 by StefanPochmann 

//Runtime: 0 ms, faster than 100.00 % of C++ online submissions for Base 7.
//Memory Usage : 6 MB, less than 50.12 % of C++ online submissions for Base 7.

//recursion

//�� ¾��


class Solution {
public:
    string convertToBase7(int num) {
        if (num < 0) {
            return "-" + convertToBase7(-num);
        }
        if (num < 7) {
            return to_string(num);
        }
        return convertToBase7(num / 7) + to_string(num % 7);
    }
}