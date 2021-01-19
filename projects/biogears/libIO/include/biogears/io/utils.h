#pragma once

#include <string>

namespace biogears {
  namespace io {
    std::string ResolvePath(const std::string& path);
    std::string generate_file_sha1( const std::string& path);
    std::string generate_sha1( char const * buffer, size_t content_size);

  }
}