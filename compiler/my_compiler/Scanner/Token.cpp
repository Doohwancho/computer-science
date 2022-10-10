#include <map>
#include <iomanip>
#include "Token.h"

using std::map;
using std::setw;
using std::left;

static map<string, Kind> stringToKind = {
  {"#unknown",    Kind::Unknown},
  {"#EndOfToken", Kind::EndOfToken},

  {"null",        Kind::NullLiteral},
  {"true",        Kind::TrueLiteral},
  {"false",       Kind::FalseLiteral},
  {"#Number",     Kind::NumberLiteral},
  {"#String",     Kind::StringLiteral},
  {"#identifier", Kind::Identifier},

  {"function",    Kind::Function},
  {"return",      Kind::Return},
  {"var",         Kind::Variable},
  {"for",         Kind::For},
  {"break",       Kind::Break},
  {"continue",    Kind::Continue},
  {"if",          Kind::If},
  {"elif",        Kind::Elif},
  {"else",        Kind::Else},
  {"print",       Kind::Print},
  {"printLine",   Kind::PrintLine},

  {"and",         Kind::LogicalAnd},
  {"or",          Kind::LogicalOr},

  {"=",           Kind::Assignment},

  {"+",           Kind::Add},
  {"-",           Kind::Subtract},
  {"*",           Kind::Multiply},
  {"/",           Kind::Divide},
  {"%",           Kind::Modulo},

  {"==",          Kind::Equal},
  {"!=",          Kind::NotEqual},
  {"<",           Kind::LessThan},
  {">",           Kind::GreaterThan},
  {"<=",          Kind::LessOrEqual},
  {">=",          Kind::GreaterOrEqual},

  {",",           Kind::Comma},
  {":",           Kind::Colon},
  {";",           Kind::Semicolon},
  {"(",           Kind::LeftParen},
  {")",           Kind::RightParen},
  {"{",           Kind::LeftBrace},
  {"}",           Kind::RightBrace},
  {"[",           Kind::LeftBraket},
  {"]",           Kind::RightBraket},
};

auto toKind(string string)->Kind {
  if (stringToKind.count(string)) //if param string exists in Kind map, return Kind of that string
    return stringToKind.at(string);
  return Kind::Unknown; //else, return undefined
}

auto toString(Kind type)->string {
  if (kindToString.count(type)) //if param Kind type exists in Kind map, 
    return kindToString.at(type); //return string that matches that Kind type.
  return "";
}

static auto kindToString = [] { //reverse map of stringToKind
  map<Kind, string> result;
  for (auto& [key, value] : stringToKind)
    result[value] = key; 
  return result;
}();




auto operator<<(ostream& stream, Token& token)->ostream& { 
  return stream << setw(12) << left << toString(token.kind) << token.string; //setw(12) -> 12칸 이후부터 출력하라는 것 
}
