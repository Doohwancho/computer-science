#include <iostream>

using namespace std;


/*
Runtime: 0 ms, faster than 100.00% of C++ online submissions for Determine Color of a Chessboard Square.
Memory Usage: 6 MB, less than 16.71% of C++ online submissions for Determine Color of a Chessboard Square.

a,b,c,d, ...��
1,2,3,4�� �ٲ㼭

���ڿ��� ���� ¦���� ����, Ȧ���� �Ͼ��

�ٵ� 'a'�� �̹� 97, Ȧ��.

�̰� �̿�

*/

class Solution {
public:
    bool squareIsWhite(string coordinates) {
        return ((int)coordinates.at(0) + (coordinates.at(1) - '0')) % 2 == 1;
    }
};



/*

solution by votrubac

�� �̰���


*/


bool squareIsWhite(string c) {
    return (c[0] + c[1]) % 2;
}