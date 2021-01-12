#include <biogears/io/utils.h>

#include <fstream>
#include <string>

#include <biogears/filesystem/path.h>

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
      filesystem::path given_path{ path };
      filesystem::path cwd{ GetCurrentWorkingDirectory() };
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
        return filesystem::normalize(origin/cwd/given_path).string();
      }
    }
  }
}