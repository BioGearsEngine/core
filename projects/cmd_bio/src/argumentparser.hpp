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
  bool exists(std::string name)
  {
    return (args.find(name) != args.end());
  }
  //--------------------------------------------------------------------------------
  std::vector<std::string> getArgList(std::string name){
    std::map<std::string, std::vector<std::string>>::iterator it = args.find(name);
	if(it == args.end()){
	  std::vector<std::string> v;
	  return v;
	}
	return it->second;
  }
  //--------------------------------------------------------------------------------
  void addArgument(std::string name)
  {
    addArgument(name, 0);
  }
  //--------------------------------------------------------------------------------
  void addArgument(std::string name, int nargs)
  {
    arg_list[name] = nargs;
  }
};