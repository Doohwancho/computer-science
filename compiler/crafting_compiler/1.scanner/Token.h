#pragma once
#include <string>
#include <iostream>

using std::string; //include로 외부 dependency에서 가져온 메서드를 using으로 쓰는 듯. import { string } from "string"; 이런 느낌?
using std::ostream;

enum class Kind { //여기가 핵심. enum으로 앞으로 나올 수 있는 모든 최소단위의 타입을 지정해준다. enum도 헤더파일에 넣네
  Unknown, EndOfToken, //undefined, '\0' for representing end of string || array

  NullLiteral, //literal = representing a fixed value in source code
  TrueLiteral, FalseLiteral,
  NumberLiteral, StringLiteral,
  Identifier, //identifier = a name that is assigned by the user for a program element such as variable, type, template, class, function or namespace.

  Function, Return,
  Variable,
  For, Break, Continue,
  If, Elif, Else,
  Print, PrintLine,

  LogicalAnd, LogicalOr,
  Assignment,
  Add, Subtract,
  Multiply, Divide, Modulo,
  Equal, NotEqual,
  LessThan, GreaterThan,
  LessOrEqual, GreaterOrEqual,

  Comma, Colon, Semicolon,
  LeftParen, RightParen,
  LeftBrace, RightBrace,
  LeftBraket, RightBraket,
};

auto toKind(string)->Kind;
auto toString(Kind)->string;

struct Token { //이게 최소 단위. 토큰:string 
  Kind kind = Kind::Unknown;
  string string;
};

auto operator<<(ostream&, Token&)->ostream&; //bit연산용 메서드인가?
