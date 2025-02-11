//첫째 줄에 다음 세 가지 중 하나를 출력한다.
//A가 B보다 큰 경우에는 '>'를 출력한다.
//A가 B보다 작은 경우에는 '<'를 출력한다.
//A와 B가 같은 경우에는 '=='를 출력한다.
    
#include <stdio.h>
    
int main() {
    int a;
    int b;
    scanf("%d", &a);
    scanf("%d", &b);
    
    if(a > b) {
        printf(">"); // 주의! printf('>'); 안된다! 반드시 ""여야 한다!
    } else if(a < b) {
        printf("<"); // 주의! printf('>'); 안된다! 반드시 ""여야 한다!
    } else {
        printf("==");
    }
    return 0;
}
