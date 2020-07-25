#include <vector>
#include <string>

#pragma once

namespace biogears {
namespace filesystem {
  std::vector<std::string> dirlist(std::string path, std::string filter = ".*" );
}
}