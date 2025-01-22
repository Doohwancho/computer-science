#include "Main.h"

//src code -> 숫자, 문자, 특수문자 등 타입에 따라 다른 토큰 부여
auto main(int argc, char** argv)->int {
  string sourceCode = R""""(
    function main() {
      print 'Hello, World!';
    }
  )"""";
  //auto: 타입 추론
  auto tokenList = scan(sourceCode);  //Scanner.cpp에 있는 메서드를 어떻게 가져왔지?
  printTokenList(tokenList);
  return 0;
}

auto printTokenList(vector<Token> tokenList)->void {
  cout << setw(12) << left << "KIND" << "STRING" << endl; //table에 KIND, STRING column 기준으로
  cout << string(23, '-') << endl;
  for (auto& token: tokenList) //어떤 Kind의 String이 있는지 출력
    cout << token << endl;
}
