#ifndef ARGUMENT_PARSER_HPP
#define ARGUMENT_PARSER_HPP
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

class ArgumentParser {
  //--------------------------------------------------------------------------------
private:
  std::map<std::string, int> arg_list;
  std::map<std::string, std::vector<std::string>> args;
  //--------------------------------------------------------------------------------
public:
  ArgumentParser() {}
  ~ArgumentParser() {}
//--------------------------------------------------------------------------------
  //!
  //! \brief Takes in command line arguments, checks them against arg_list, and inserts them to args.
  //! \param argc Number of arguments
  //! \param argv Pointers to arguments
  //! 
  void parse(int argc, char** argv)
  {
    for (int i = 1; i < argc; i++) {
      std::string name(*(argv + i));
      std::map<std::string, int>::iterator it = arg_list.find(name);
      if (it != arg_list.end()) {
        int k = i + it->second;
        std::vector<std::string> v;
        while (i < k) {
          ++i;
          std::string str(*(argv + i));
          v.push_back(str);
        }
        args[name] = v;
      } else {
        std::cout << "Error: Input not recognized" << std::endl;
      }
    }
  }
  //--------------------------------------------------------------------------------
  //!
  //! \brief Checks if name is contained in args
  //! \param name string to search args for
  //! \return bool true if name is present in args, false otherwise
  //! 
  bool exists(std::string name)
  {
    return (args.find(name) != args.end());
  }
  //--------------------------------------------------------------------------------
  //!
  //! \brief Searches for std::string name in args, if name is present returns 
  //!        string vector of inputs mapped to name. Otherwise returns empty vector.
  //! \param name string to search args for 
  //! \return vector of strings
  //! 
  std::vector<std::string> getArgList(std::string name){
    std::map<std::string, std::vector<std::string>>::iterator it = args.find(name);
	if(it == args.end()){
	  std::vector<std::string> v;
	  return v;
	}
	return it->second;
  }
  //--------------------------------------------------------------------------------
  //!
  //! \brief If no int specifying number of args for name is provided, then the argumentparser assumes the number is 0.
  //! \param name string to add to arg_list
  //! 
  void addArgument(std::string name)
  {
    addArgument(name, 0);
  }
  //--------------------------------------------------------------------------------
  //!
  //! \brief 
  //! \param name string to add to arg_list 
  //! \param nargs number of arguments that directly follow name in command line input.
  //! 
  void addArgument(std::string name, int nargs)
  {
    arg_list[name] = nargs;
  }
};

#endif //ARGUMENT_PARSER_HPP