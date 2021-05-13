#include "dir.h"


#include <regex>
#include <biogears/filesystem/path.h>
#include <biogears/string/manipulation.h>
#include <biogears/io/io-manager.h>
#include <exception>
#include <iostream>

namespace biogears {
namespace filesystem {
  std::vector<std::string> dirlist(std::string path, std::string filter)
  {
    std::vector<std::string> results;
    for ( auto& file : ::biogears::ListFiles(path,filter) ){
         results.push_back(file);
    }
    return results;
  }
}
}
