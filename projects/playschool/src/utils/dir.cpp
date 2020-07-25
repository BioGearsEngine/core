#include "dir.h"

#include <dirent.h>
#include <regex>
#include <biogears/filesystem/path.h>
#include <biogears/string/manipulation.h>
#include <exception>
#include <iostream>

namespace biogears {
namespace filesystem {
  std::vector<std::string> dirlist(std::string path, std::string filter)
  {
    std::vector<std::string> result;
    DIR* dir = nullptr;
    dirent* ent = nullptr;
    if ((dir = opendir(path.c_str())) != NULL) {
      /* print all the files and directories within directory */

      try {
        std::regex re{ filter };
        std::smatch m;
        while ((ent = readdir(dir)) != NULL) {
          std::string file = ent->d_name;
          if (std::regex_match(file, m, re)) {
            result.push_back( (biogears::filesystem::path(path) / std::string(ent->d_name)).string());
          }
        }
      } catch (...) {
        throw std::runtime_error("Unable to compile given regex " + filter);
      }
      closedir(dir);
    } else {
      throw std::runtime_error(biogears::asprintf("Unable to list directory containing %s", path.c_str()));
    }
    return result;
  }
}
}
