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