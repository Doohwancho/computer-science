#include <iostream>
#include <vector>

using namespace std;

int main() {
    int a, max = -1, x = 0, y = 0;
    vector<vector<int>> v(9, vector<int>(9));

    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            cin >> a;
            v[i][j] = a;
            if(a > max) {
                max = a;
                x = i+1;
                y = j+1;
            }
        }
    }

    cout << max << endl << x << " " << y << endl;

    return 0;
}