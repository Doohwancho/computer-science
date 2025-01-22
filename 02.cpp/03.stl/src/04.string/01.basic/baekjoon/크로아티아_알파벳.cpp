#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    // vector<string> croatian = {"c=","c-","dz=","d-","lj","nj","s=","z="};
    string croatianArray[] = {"c=", "c-", "dz=", "d-", "lj", "nj", "s=", "z="};
    vector<string> croatian(croatianArray, croatianArray + sizeof(croatianArray) / sizeof(croatianArray[0]));

    int idx;
    string str;
    cin >> str;
    for(int i = 0; i < croatian.size(); i++)
    {
        while(1){
            idx = str.find(croatian[i]);
            if(idx == string::npos)
                break;
            str.replace(idx,croatian[i].length(),"#");
        }
    }
    cout << str.length();
    return 0;
}