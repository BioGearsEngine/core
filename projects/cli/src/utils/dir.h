#include <vector>
#include <string>

#include <biogears/config.h>

#pragma once

OPEN_BIOGEARS_NAMESPACE
namespace filesystem {
  std::vector<std::string> dirlist(std::string path, std::string filter = ".*" );
}
CLOSE_BIOGEARS_NAMESPACE