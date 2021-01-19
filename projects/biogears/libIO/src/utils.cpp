#include <biogears/io/utils.h>

#include <fstream>
#include <iostream>
#include <string>

#include <biogears/filesystem/path.h>
#include <biogears/io/sha1.h>

namespace biogears {
namespace io {
  std::string g_working_dir = "";

  //---------------------------------------------------------------------------
  std::string GetCurrentWorkingDirectory()
  {
    return g_working_dir;
  }

  void SetCurrentWorkingDirectory(std::string working_dir)
  {
    g_working_dir = std::move(working_dir);
  }

  void SetCurrentWorkingDirectory(const char* working_dir)
  {
    g_working_dir = working_dir;
  }

  std::string ResolvePath(const std::string& path)
  {
    filesystem::path given_path { path };
    filesystem::path cwd { GetCurrentWorkingDirectory() };
    if (path.empty()) {
      return "";
    } else if (given_path.is_absolute()) {
      return given_path.string();
    } else if (filesystem::normalize(cwd).is_absolute()) {

      return filesystem::normalize(cwd / given_path).string();
    } else {
      auto origin = filesystem::path::getcwd();
      cwd;
      given_path;
      return filesystem::normalize(origin / cwd / given_path).string();
    }
  }

  //-----------------------------------------------------------------------------
  std::string generate_file_sha1(const std::string& path)
  {
    std::string content;
    std::string result = std::string( 40, '\0' );
    sha1 hash;

    std::ifstream file { path, std::ios::in | std::ios::binary };
    if (file) {

      auto start = file.tellg();
      file.seekg(0,file.end);
      auto end  = file.tellg();
      auto length = end - start;
      file.seekg(0,file.beg);

      content.resize(length);
      file.read(&content[0], length);
      file.close();

      hash.add(&content[0]);
      hash.finalize();
      hash.print_hex(&result[0]);
    }

    return result;
  }
  //-----------------------------------------------------------------------------
  std::string generate_sha1(char const* buffer, size_t content_size)
  {
    std::string result = std::string( 40, '\0' );
    sha1 hash;

    hash.add(buffer,content_size);
    hash.finalize();
    hash.print_hex(&result[0]);

    return result;
  }
  //-----------------------------------------------------------------------------
}
}