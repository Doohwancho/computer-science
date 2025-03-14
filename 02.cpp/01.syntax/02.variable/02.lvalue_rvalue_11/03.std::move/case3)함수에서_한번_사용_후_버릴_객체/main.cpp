#include <iostream>
#include <string>

std::string processAndForget(std::string data) {
    // data 처리...
    
    // 결과 만들기
    std::string result = "처리 결과: " + data;
    
    // data는 이제 필요 없음 -> 이동으로 결과에 내용 전달
    return std::move(result); // 이동 리턴 (사실 요즘 컴파일러는 이것도 자동으로 해줌)
}
