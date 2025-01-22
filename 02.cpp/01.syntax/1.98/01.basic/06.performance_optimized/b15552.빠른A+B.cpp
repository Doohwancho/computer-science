#include <ios>
#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(false); //c, c++의 표즘 스트림 동기화 해제 
    std::cin.tie(NULL);

    int T, a, b;
    
    std::cin >> T;
    for(int i = 0; i < T; i++){
        std::cin >> a >> b;
        std::cout << a + b << '\n';
        //case1) endl
        // std::cout << a + b << std::endl; //실패!
        //왜?
        //endl은 개행문자를 출력할 뿐만 아니라 출력 버퍼를 비우는 역할까지 합니다. 
        //그래서 출력한 뒤 화면에 바로 보이게 할 수 있는데, 그 버퍼를 비우는 작업이 매우 느립니다. 
        //게다가 온라인 저지에서는 화면에 바로 보여지는 것은 중요하지 않고 무엇이 출력되는가가 중요하기 때문에 
        //버퍼를 그렇게 자주 비울 필요가 없습니다. 
        //그래서 endl을 '\n'으로 바꾸는 것만으로도 굉장한 시간 향상이 나타납니다
    }

    return 0;
}