#ifndef CMD_BIO_TOKENIZER_H
#define CMD_BIO_TOKENIZER_H
//**********************************************************************************
//Copyright 2015 Applied Research Associates, Inc.
//Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//this file except in compliance with the License.You may obtain a copy of the License
//at :
//http://www.apache.org/licenses/LICENSE-2.0
//Unless required by applicable law or agreed to in writing, software distributed under
//the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//CONDITIONS OF ANY KIND, either express or implied.See the License for the
//specific language governing permissions and limitations under the License.
//**************************************************************************************

#include <list>
#include <string>
#include <vector>

namespace biogears {

enum class ETokenClass { Undefined,
                         Whitespace,
                         Newline,
                         Comment,
                         Word,
                         Number,
                         Symbol };

struct Token {
  ETokenClass type;
  std::string value;
  size_t line;
  size_t column;
  size_t index;
  Token();
  explicit Token(const ETokenClass c, const std::string& s, size_t line=0, size_t column=0, size_t i = 0);
  explicit Token(const ETokenClass c, const char s, size_t line=0, size_t column=0, size_t i = 0);

  Token(const Token& obj) = default;
  Token(Token&& obj) = default;
  Token& operator=(const Token& obj) = default;
  Token& operator=(Token&& obj) = default;

  bool is_double() const;
  bool is_integral() const;

  double as_double();
  int as_integral();

  bool operator==(const std::string& rhs) const;
  bool operator!=(const std::string& rhs) const;

  char operator[](const size_t);
};

bool operator==(const std::string& lhs, const Token& rhs);
bool operator!=(const std::string& lhs, const Token& rhs);

class Tokenizer {

public:
  Tokenizer();
  Tokenizer(const std::string& file);
  ~Tokenizer() = default;

  bool tokenize(std::istream& os);
  void clear();

  using token_list = std::list<Token>;
  using iterator = token_list::iterator;
  using const_iterator = token_list::const_iterator;

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

private:
  token_list _tokens;
};

inline std::ostream& operator<<(std::ostream& out, const Token& t)
{
  out << t.value;
  return out;
}
}
#endif //CMD_BIO_TOKENIZER_H