#pragma once //pragma란,  컴파일에게 그 뒤에오는 내용에 따라 어떤일을 하라는 전처리명령. #pragma once = 한번 컴파일 되면 더 이상 컴파일 하지 말라.

//#include: import outer dependency
//Main.h 는 메인 헤더파일이니까, 다른 헤더파일에서 쓰일 모든 import를 여기에서 다 받는 듯? 심지어 개발자가만든 Token.h까지도?
#include <string> 
#include <vector>
#include <iomanip>
#include <iostream>
#include "Token.h"

using std::cout;
using std::endl;
using std::setw;
using std::left;
using std::string;
using std::vector;

auto scan(string)->vector<Token>; //Scanner.cpp에 있는 메서드를 여기에서 정의하면, 가져와서 쓸 수 있나보네? Main.h는 Token.h와 이어져 있고, Scanner.cpp에서 #include Token.h 니까? 근데 이 라인 지우면 에러날까?
auto printTokenList(vector<Token>)->void;
