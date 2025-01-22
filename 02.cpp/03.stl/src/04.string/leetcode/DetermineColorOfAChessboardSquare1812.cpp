#include <iostream>

using namespace std;


/*
Runtime: 0 ms, faster than 100.00% of C++ online submissions for Determine Color of a Chessboard Square.
Memory Usage: 6 MB, less than 16.71% of C++ online submissions for Determine Color of a Chessboard Square.

a,b,c,d, ...를
1,2,3,4로 바꿔서

숫자와의 합이 짝수면 검정, 홀수면 하얀색

근데 'a'는 이미 97, 홀수.

이걸 이용

*/

class Solution {
public:
    bool squareIsWhite(string coordinates) {
        return ((int)coordinates.at(0) + (coordinates.at(1) - '0')) % 2 == 1;
    }
};



/*

solution by votrubac

와 이거지


*/


bool squareIsWhite(string c) {
    return (c[0] + c[1]) % 2;
}