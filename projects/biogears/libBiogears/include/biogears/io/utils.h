#pragma once

#include <biogears/exports.h>
#include <string>

namespace biogears {
  namespace io {
    BIOGEARS_API std::string GetCurrentWorkingDirectory();
    BIOGEARS_API void SetCurrentWorkingDirectory(const char* working_dir);
    BIOGEARS_API void SetCurrentWorkingDirectory(std::string working_dir);
    BIOGEARS_API std::string generate_file_sha1( const std::string& path);
    BIOGEARS_API std::string generate_sha1( char const * buffer, size_t content_size);
    BIOGEARS_API std::string ResolvePath(const std::string& path);
  }
}