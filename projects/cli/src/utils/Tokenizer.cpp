#include "Tokenizer.h"

#include <cctype>
#include <cerrno>
#include <fstream>
#include <list>
#include <regex>

#include <biogears/string/manipulation.h>

namespace biogears {

Token::Token()
  : type(ETokenClass::Undefined)
  , value()
{
}
//-----------------------------------------------------------------------------
Token::Token(const ETokenClass c, const std::string& s, size_t l, size_t col, size_t i)
  : type(c)
  , value(s)
  , line(l)
  , column(col)
  , index(i)
{
}
//-----------------------------------------------------------------------------
Token::Token(const ETokenClass c, const char s, size_t l, size_t col, size_t i)
  : type(c)
  , value()
  , line(l)
  , column(col)
  , index(i)
{
  value.push_back(s);
}
//-----------------------------------------------------------------------------
bool Token::is_double() const
{
  if (type == ETokenClass::Number) {

    char* end = nullptr;
    double val = strtod(value.c_str(), &end);
    errno = 0;
    return end != value.c_str() && ERANGE != errno;
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
  size_t current_line = 0;
  while (std::getline(os, line)) {
    ++current_line;
    trim(line);
    if (line.empty()) {
      continue;
    }
    line += "\n";
    Token workItem{ ETokenClass::Undefined, "", current_line, 0 };
    size_t current_column = 0;
    for (auto itr = line.begin(); itr != line.end(); ++itr) {
      ++current_column;
      auto& cur = *itr;

      if (workItem.type == ETokenClass::Undefined) {
        if (cur == '\n') {
          _tokens.emplace_back(ETokenClass::Newline, cur, current_line, current_column);
        } else if (std::isspace(cur)) {
          workItem.type = ETokenClass::Whitespace;
          workItem.line = current_line;
          workItem.column = current_column;
          workItem.value.push_back(cur);
        } else if (cur == '#') {
          break;
        } else if (std::isdigit(cur)) {
          workItem.type = ETokenClass::Number;
          workItem.line = current_line;
          workItem.column = current_column;
          workItem.value.push_back(cur);
        } else if (std::isalpha(cur)) {
          workItem.type = ETokenClass::Word;
          workItem.line = current_line;
          workItem.column = current_column;
          workItem.value.push_back(cur);
        } else {
          _tokens.emplace_back(ETokenClass::Symbol, cur, current_line, current_column);
        }
      } else if (cur == '\n') {
        workItem.index = _tokens.size();
        _tokens.push_back(workItem);
        _tokens.emplace_back(ETokenClass::Newline, cur, current_line, current_column, _tokens.size());
        workItem = Token();
      } else if (workItem.type != ETokenClass::Whitespace && std::isspace(cur)) {
        workItem.index = _tokens.size();
        _tokens.push_back(workItem);
        workItem = Token(ETokenClass::Whitespace, cur, current_line, current_column, _tokens.size());
      } else if (cur == '#') {
        workItem.index = _tokens.size();
        _tokens.push_back(workItem);
        workItem = Token();
        break;
      } else if (workItem.type == ETokenClass::Whitespace) {
        if (std::isspace(cur)) {
          workItem.value.push_back(cur);
        } else if (std::isalpha(cur)) {
          workItem.index = _tokens.size();
          _tokens.push_back(workItem);
          workItem = Token{ ETokenClass::Word, cur, current_line, current_column };
        } else if (std::isdigit(cur)) {
          workItem.index = _tokens.size();
          _tokens.push_back(workItem);
          workItem = Token{ ETokenClass::Number, cur, current_line, current_column };
        } else {
          workItem.index = _tokens.size();
          _tokens.push_back(workItem);
          _tokens.emplace_back(ETokenClass::Symbol, cur, current_line, current_column, _tokens.size());
          workItem = Token();
        }
      } else if (workItem.type == ETokenClass::Number) {
        if (std::isdigit(cur)) {
          workItem.value.push_back(cur);
        } else if (std::isalnum(cur)) {
          workItem.value.push_back(cur);
          workItem.type = ETokenClass::Word;
        } else {
          workItem.index = _tokens.size();
          _tokens.push_back(workItem);
          _tokens.emplace_back(ETokenClass::Symbol, cur, current_line, current_column, _tokens.size());
          workItem = Token();
        }
      } else if (workItem.type == ETokenClass::Word) {
        if (std::isalnum(cur)) {
          workItem.value.push_back(cur);
        } else if (cur == '_') {
          workItem.value.push_back(cur);
        } else {
          workItem.index = _tokens.size();
          _tokens.push_back(workItem);
          _tokens.emplace_back(ETokenClass::Symbol, cur, current_line, current_column, _tokens.size());
          workItem = Token();
        }
      }
    }
    if (workItem.type != ETokenClass::Undefined) {
      workItem.index = _tokens.size();
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
