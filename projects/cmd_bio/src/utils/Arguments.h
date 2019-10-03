#ifndef CMD_BIO_UTILS_ARGUMENT_H
#define CMD_BIO_UTILS_ARGUMENT_H

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
#include <iostream>
#include <map>
#include <string>
#include <vector>

//! \file
//! \brief Case insensitive argument parser based on cmake_handle_arguments

namespace biogears  {
class Arguments {
public:

  explicit Arguments( const std::vector<std::string>&, const std::vector<std::string>&, const std::vector<std::string>&);
  ~Arguments() = default;

  bool parse(int argc, char* argv[]);
  bool parse(const std::vector<std::string>& args);

  bool isOption(std::string) const;
  bool isKeyword(std::string) const;
  bool isMultiWord(std::string) const;
  bool exists(const std::string&) const;

  using OptionValue = bool;
  using KeywordValue = std::string;
  using MultiwordValue = std::vector<std::string>;

  OptionValue Option(std::string) const;
  KeywordValue Keyword(std::string) const;
  MultiwordValue MultiWord(std::string) const;

  bool KeywordFound(std::string) const;
  bool MultiWordFound(std::string) const;

  bool empty() const { return _empty;
                     }
protected:
  std::map<std::string,OptionValue>    _options;
  std::map<std::string,KeywordValue>   _keywords;
  std::map<std::string,MultiwordValue> _multiwords;
  bool _empty = true;
};

}

#endif //ARGUMENT_PARSER_HPP
