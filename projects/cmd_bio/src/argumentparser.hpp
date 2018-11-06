#include <map>
#include <string>
#include <vector>

class ArgumentParser {
private:
  std::map<std::string,int> args;

public:
  ArgumentParser(){}
  ~ArgumentParser(){}
  void parse(int argc, char** argv) {
    for(int i = 1;i < argc;i++){
      std::string str(*(argv + i));
      args[str] = i;
    }
  }
  bool exists(std::string str) {
  	return (args.find(str) != args.end());
  }

};