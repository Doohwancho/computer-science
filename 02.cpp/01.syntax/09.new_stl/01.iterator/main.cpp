#include <iostream>
#include <vector>
#include <map>

int main() {
    std::vector<int> v = {1,2,3,4,5};
    
    //case1) 벡터 iteration using iterator 
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        // std::cout << it << " "; //iterator에서 << operator가 정의되지 않아서 print를 못하네. *it로만 프린트 가능하네. 
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "---------------------------" << std::endl;

    // case2) 더 간단한 범위 기반 for문
    for (auto& val : v) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    std::cout << "---------------------------" << std::endl;

    // case3) map도 함 해볼까?
    std::map<std::string, int> m;
    m["키"] = 30;

    // map의 반복자는 pair<const Key, Value> 타입을 가리킴
    for (auto it = m.begin(); it != m.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    std::cout << "---------------------------" << std::endl;


    // case4) iterator 객체 넘겨줘서 지우기?
    auto it = m.find("키");
    if (it != m.end()) {
        m.erase(it); // 특정 위치의 요소 삭제
    }
    

    return 0;
}
