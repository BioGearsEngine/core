#include "dir.h"


#include <regex>
#include <exception>
#include <iostream>

#include <biogears/config.h>
#include <biogears/filesystem/path.h>
#include <biogears/string/manipulation.h>
#include <biogears/io/io-manager.h>

OPEN_BIOGEARS_NAMESPACE
namespace filesystem {
  std::vector<std::string> dirlist(std::string path, std::string filter)
  {
    std::vector<std::string> results;
    for ( auto& file : ::BIOGEARS_NAMESPACE ListFiles(path,filter) ){
         results.push_back(file);
    }
    return results;
  }
}
  CLOSE_BIOGEARS_NAMESPACE
