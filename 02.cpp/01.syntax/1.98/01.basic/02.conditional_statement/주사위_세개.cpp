#include <iostream>
#include <math.h>

//fail 
//However, this approach doesn't handle the case, 
//when 'b' and 'c' have the same value, but 'a' is different. 
// int main() {
//     int a,b,c;
//     std::cin >> a >> b >> c;

//     if(a == b && b == c) {
//         std::cout << 10000 + a * 1000 << std::endl;
//     } else if (a == b || a == c || b == c) { //error! 
//         std::cout << 1000 + a * 100 << std::endl;
//     } else {
//        int max = std::max(a, std::max(b,c));
//         std::cout << max * 100 << std::endl;
//     }
//     return 0;
// }


//solution1
//a==b || a == c 한 다음, b==c 한 경우 따로 처리해야 한다.
using namespace std;

int main(){
    int a, b, c;
    cin>>a>>b>>c;
    if(a == b && b == c){
        cout<<10000 + a*1000;
    }
    else if(a == b || a == c){
        cout<<1000 + a*100;
    }
    else if(b == c){
        cout<<1000 + b*100;
    }
    else{
        cout<<max(max(a, b), c)*100;
    }
    return 0;
}

//solution2 
// a==b, b==c, a==c 따로 처리하는 방식 
// #include <iostream>
// #include <algorithm>

// int main() {
//     int a, b, c;
//     std::cin >> a >> b >> c;

//     if (a == b && b == c) {
//         std::cout << 10000 + a * 1000 << std::endl;
//     } else if (a == b) {
//         std::cout << 1000 + a * 100 << std::endl;
//     } else if (a == c) {
//         std::cout << 1000 + a * 100 << std::endl;
//     } else if (b == c) {
//         std::cout << 1000 + b * 100 << std::endl;
//     } else {
//         int max = std::max(a, std::max(b, c));
//         std::cout << max * 100 << std::endl;
//     }

//     return 0;
// } 