#include <vector>
#include <string>
#include <iostream>
#include "Token.h"

using std::cout;
using std::string;
using std::vector;

enum class CharType {
  Unknown,
  WhiteSpace,
  NumberLiteral,
  StringLiteral,
  IdentifierAndKeyword,
  OperatorAndPunctuator,
};

static auto scanNumberLiteral()->Token;
static auto scanStringLiteral()->Token;
static auto scanIdentifierAndKeyword()->Token;
static auto scanOperatorAndPunctuator()->Token;
static auto getCharType(char)->CharType;
static auto isCharType(char, CharType)->bool;

static string::iterator current;

auto scan(string sourceCode)->vector<Token> {
  vector<Token> result;
  sourceCode += '\0'; //end of line
  current = sourceCode.begin();
  while (*current != '\0') { 
    switch (getCharType(*current)) {
      case CharType::WhiteSpace: {
        current += 1;
        break;
      }
      case CharType::NumberLiteral: {
        result.push_back(scanNumberLiteral()); //123 -> Token{Kind::NumberLiteral, '123'};
        break;
      }
      case CharType::StringLiteral: {
        result.push_back(scanStringLiteral()); //"Hello World" -> Token{Kind::StringLiteral, "Hello World"};
        break;
      }
      case CharType::IdentifierAndKeyword: {
        result.push_back(scanIdentifierAndKeyword()); //숫자+알파벳 -> Token{kind, string};
        break;
      }
      case CharType::OperatorAndPunctuator: {
        result.push_back(scanOperatorAndPunctuator()); //{}, [], =, +, 등 특수문자 ->  Token{toKind(string), string}
        break;
      }
      default: {
        cout << *current << " 사용할 수 없는 문자입니다.";
        exit(1);
      }
    }
  }
  result.push_back({Kind::EndOfToken});
  return result;
}

auto scanNumberLiteral()->Token {
  string string;
  while (isCharType(*current, CharType::NumberLiteral)) //부모에서 넘어온 *current를 그대로 쓰나보네
    string += *current++;
  if (*current == '.') {
    string += *current++;
    while (isCharType(*current, CharType::NumberLiteral))
      string += *current++;
  }
  return Token{Kind::NumberLiteral, string};
}

auto scanStringLiteral()->Token {
  string string;
  current++; //맨 앞에 "나 ' 한칸 건너뛰어야 하니까
  while (isCharType(*current, CharType::StringLiteral))
    string += *current++;
  if (*current != '\'') {
    cout << "문자열의 종료 문자가 없습니다."; //"hello world! 에서 맨 마지막에 closing paranthesis 없으면 
    exit(1); //throw compile error!
  }
  current++; //맨 뒤에 " 한칸 띄기
  return Token{Kind::StringLiteral, string};
}

auto scanIdentifierAndKeyword()->Token {
  string string;
  while (isCharType(*current, CharType::IdentifierAndKeyword))
    string += *current++;
  auto kind = toKind(string);
  if (kind == Kind::Unknown)
    kind = Kind::Identifier;
  return Token{kind, string};
}

auto scanOperatorAndPunctuator()->Token {
  string string;
  while (isCharType(*current, CharType::OperatorAndPunctuator))
    string += *current++;
  while (string.empty() == false && toKind(string) == Kind::Unknown) {
    string.pop_back();
    current--;
  }
  if (string.empty()) {
    cout << *current << " 사용할 수 없는 문자입니다.";
    exit(1);
  }
  return Token{toKind(string), string};
}

auto getCharType(char c)->CharType {
  if (' ' == c || '\t' == c || '\r' == c || '\n' == c) {
    return CharType::WhiteSpace;
  }
  if ('0' <= c && c <= '9') {
    return CharType::NumberLiteral;
  }
  if (c == '\'') {
    return CharType::StringLiteral;
  }
  if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' ) {
    return CharType::IdentifierAndKeyword;
  }
  if (33 <= c && c <= 47 && c != '\'' ||
      58 <= c && c <= 64 ||
      91 <= c && c <= 96 ||
      123 <= c && c <= 126) {
    return CharType::OperatorAndPunctuator;
  }
  return CharType::Unknown;
}

auto isCharType(char c, CharType type)->bool {
  switch (type) {
    case CharType::NumberLiteral: {
      return '0' <= c && c <= '9'; //숫자
    }
    case CharType::StringLiteral: {
      return 32 <= c && c <= 126 && c != '\''; //space 등 온갖 특수 문자 + 0~9 number + 알파벳 대소문자
    }
    case CharType::IdentifierAndKeyword: {
      return '0' <= c && c <= '9' ||
             'a' <= c && c <= 'z' ||
             'A' <= c && c <= 'Z'; //숫자+ 알파벳 대소문자
    }
    case CharType::OperatorAndPunctuator: {
      return 33 <= c && c <= 47 || //!,",#, ... , /
              58 <= c && c <= 64 || //:,;,<, ... @ because 48~57 is 0~9
              91 <= c && c <= 96 || //[,\, ], ... `
              123 <= c && c <= 126; //{,|,}
    }
    default: {
      return false;
    }
  }
}
