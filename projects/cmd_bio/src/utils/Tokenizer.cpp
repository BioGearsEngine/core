#include "Tokenizer.h"
#include "string-helpers.h"

#include <cctype>
#include <fstream>
#include <list>
#include <regex>

namespace biogears {

Token::Token()
  : type(ETokenClass::Undefined)
  , value()
{
}
//-----------------------------------------------------------------------------
Token::Token(const ETokenClass c, const std::string& s)
  : type(c)
  , value(s)
{
}
//-----------------------------------------------------------------------------
Token::Token(const ETokenClass c, const char s)
  : type(c)
  , value()
{
  value.push_back(s);
}
//-----------------------------------------------------------------------------
bool Token::is_double() const
{
  if (type == ETokenClass::Number) {

    char* end = nullptr;
    double val = strtod(value.c_str(), &end);
    return end != value.c_str() && val != HUGE_VAL;
  }
  return false;
}
//-----------------------------------------------------------------------------
bool Token::is_integral() const
{
  if (type == ETokenClass::Number) {

    if (std::isdigit(value[0]) || (value.size() > 1 && (value[0] == '-' || value[0] == '+'))) {
      for (std::string::size_type i{ 1 }; i < value.size(); ++i)
        if (!std::isdigit(value[i]))
          return false;

      return true;
    }
    return false;
  }
  return false;
}
//-----------------------------------------------------------------------------
double Token::as_double()
{
  return strtod(value.c_str(), nullptr);
}
//-----------------------------------------------------------------------------
int Token::as_integral()
{
  return strtol(value.c_str(), nullptr, 0);
}
//-----------------------------------------------------------------------------
bool Token::operator==(const std::string& rhs) const
{
  return value == rhs;
}
//-----------------------------------------------------------------------------
bool Token::operator!=(const std::string& rhs) const
{
  return !(*this == rhs);
}
//-----------------------------------------------------------------------------
char Token::operator[](const size_t index)
{
  return value[index];
}
//-----------------------------------------------------------------------------
bool operator==(const std::string& lhs, const Token& rhs)
{
  return rhs == lhs;
}
//-----------------------------------------------------------------------------
bool operator!=(const std::string& lhs, const Token& rhs)
{
  return rhs != lhs;
}
//-----------------------------------------------------------------------------
Tokenizer::Tokenizer()
{
}
//-----------------------------------------------------------------------------
Tokenizer::Tokenizer(const std::string& fp)
{
  std::ifstream file{ fp };
  if (file.is_open()) {
    tokenize(file);
  }
}

//-----------------------------------------------------------------------------
//! Tokenize
//! \brief
bool Tokenizer::tokenize(std::istream& os)
{
  std::string line;
  char delimiter = '\0';
  while (std::getline(os, line)) {
    trim(line);
    if (line.empty()) {
      continue;
    }
    Token workItem{ ETokenClass::Undefined, "" };
    for (auto itr = line.begin(); itr != line.end(); ++itr) {
      auto& cur = *itr;

      if (workItem.type == ETokenClass::Undefined) {
        if (cur == '\n') {
          _tokens.emplace_back(ETokenClass::Newline, cur);
        } else if (std::isspace(cur)) {
          workItem.type = ETokenClass::Whitespace;
          workItem.value.push_back(cur);
        } else if (cur == '#') {
          break;
        } else if (std::isdigit(cur)) {
          workItem.type = ETokenClass::Number;
          workItem.value.push_back(cur);
        } else if (std::isalpha(cur)) {
          workItem.type = ETokenClass::Word;
          workItem.value.push_back(cur);
        } else {
          _tokens.emplace_back(ETokenClass::Symbol, cur);
        }
      } 
      else if (cur == '\n') {
        _tokens.push_back(workItem);
        _tokens.emplace_back(ETokenClass::Newline, cur);
        workItem = Token();
      } 
      else if (workItem.type != ETokenClass::Whitespace && std::isspace(cur)) {
        _tokens.push_back(workItem);
        workItem = Token(ETokenClass::Whitespace,cur);
      } 
      else if (cur == '#') {
        _tokens.push_back(workItem);
        workItem = Token();
        break;
      } 
      else if (workItem.type == ETokenClass::Whitespace) {
        if (std::isspace(cur)) {
          workItem.value.push_back(cur);
        } else if (std::isalpha(cur)) {
          _tokens.push_back(workItem);
          workItem = Token{ ETokenClass::Word, cur };
        } else if (std::isdigit(cur)) {
          _tokens.push_back(workItem);
          workItem = Token{ ETokenClass::Number, cur };
        } else {
          _tokens.push_back(workItem);
          _tokens.emplace_back(ETokenClass::Symbol, cur);
          workItem = Token();
        }
      } 
      else if (workItem.type == ETokenClass::Number) {
        if (std::isdigit(cur)) {
          workItem.value.push_back(cur);
        } else if (std::isalpha(cur)) {
          _tokens.push_back(workItem);
          workItem = Token{ ETokenClass::Word, cur };
        } else {
          _tokens.push_back(workItem);
          _tokens.emplace_back(ETokenClass::Symbol, cur);
          workItem = Token();
        }
      } 
      else if (workItem.type == ETokenClass::Word) {
        if (std::isalnum(cur)) {
          workItem.value.push_back(cur);
        }  else {
          _tokens.push_back(workItem);
          _tokens.emplace_back( ETokenClass::Symbol, cur );
          workItem = Token();
        }
      }
    }
    if (workItem.type != ETokenClass::Undefined) {
      _tokens.push_back(workItem);
    }
  }

  return true;
}
//-----------------------------------------------------------------------------
void Tokenizer::clear()
{
  _tokens.clear();
}
//-----------------------------------------------------------------------------
auto Tokenizer::begin() -> iterator
{
  return _tokens.begin();
}
//-----------------------------------------------------------------------------
auto Tokenizer::begin() const -> const_iterator
{
  return const_cast<Tokenizer*>(this)->begin();
}
//-----------------------------------------------------------------------------
auto Tokenizer::end() -> iterator
{
  return _tokens.end();
}
//-----------------------------------------------------------------------------
auto Tokenizer::end() const -> const_iterator
{
  return const_cast<Tokenizer*>(this)->end();
}
//-----------------------------------------------------------------------------
}
