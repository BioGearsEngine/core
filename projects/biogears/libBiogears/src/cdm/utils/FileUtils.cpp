/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include <biogears/cdm/utils/FileUtils.h>

#include <cstdio>
#include <dirent.h>
#include <regex>

#include <biogears/filesystem/path.h>

#if defined(_MSC_VER) || defined(__MINGW64_VERSION_MAJOR)

#define MAXPATH MAX_PATH
#define GETCWD _getcwd
#define MKDIR(x) _mkdir(x)
#define RMDIR(x) _rmdir(x)
#else

#define MAXPATH PATH_MAX
#define GETCWD getcwd
#define MKDIR(x) mkdir(x, 0755)
#define RMDIR(x) rmdir(x)

#endif

namespace biogears {

std::string g_working_dir = "";

std::string Replace(const std::string& original, const std::string& replace, const std::string& withThis)
{
  size_t idx = 0;
  std::string s = original;
  idx = s.find(replace);
  if (idx != std::string::npos) {
    s.erase(idx, replace.length());
    s.insert(idx, withThis);
  }
  return s;
}

bool CreateFilePath(const std::string& path)
{
  ScopedFileSystemLock lock;

  if (path.empty()) {
    return false;
  }

  std::string buffer = path;
  std::vector<std::string> folderLevels;
  char* c_str = (char*)buffer.c_str();

  // Point to end of the string
  char* strPtr = &c_str[strlen(c_str) - 1];

  // Break out each directory into our vector
  do {
    do {
      strPtr--;
    } while ((*strPtr != '\\') && (*strPtr != '/') && (strPtr >= c_str));
    folderLevels.push_back(std::string(strPtr + 1));
    strPtr[1] = 0;
  } while (strPtr >= c_str);

  std::string destDir = "";

  std::string dir;

  // Create the folders iteratively, backwards
  for (size_t i = folderLevels.size() - 1; i >= 1; i--) {
    dir = folderLevels.at(i);
    if (dir == "/" || dir == "\\")
      continue;
    destDir += dir;
    MKDIR(destDir.c_str());
  }
  return true;
}

bool CreateFilePath(const char* path)
{
  std::string path_str{ path };
  return CreateFilePath(path_str);
}
void ListFiles(const std::string& dir, std::vector<std::string>& files, const std::string& regex, bool recurse)
{
  DIR* d;
  dirent* ent;
  std::string filename;
  std::regex mask{ regex };
  if ((d = opendir(dir.c_str())) != nullptr) {
    while ((ent = readdir(d)) != nullptr) {
      size_t nameLength = strlen(ent->d_name);

      if (ent->d_name[0] == '.' && ((nameLength == 1) || (nameLength == 2 && ent->d_name[1] == '.')))
        continue;
      filename = dir;
      filename += "/";
      filename += ent->d_name;

      if (!IsDirectory(ent)) {
        if (std::regex_search(filename, mask))
          files.push_back(filename);
      } else {
        if (recurse) {
          ListFiles(filename, files, regex, recurse);
        }
      }
    }
  }
}

bool IsAbsolutePath(const std::string& path)
{
  return filesystem::path{ path }.is_absolute();
}

bool IsAbsolutePath(const char* path)
{
  return filesystem::path{ path }.is_absolute();
}

std::string ResolveAbsolutePath(const std::string& path)
{
  filesystem::path given_path{ path };
  filesystem::path cwd{ GetCurrentWorkingDirectory() };

  return ((given_path.is_absolute()) ? given_path
                                     : (filesystem::path{ cwd }.is_absolute()) ? (cwd / given_path).make_normal()
                                                                               :(given_path).make_normal())
    .string();
}
//!
//!  \param const char* path Path to be resolved
//!  \brief This call is very unsafe when using threading. The lifetime of the char* returned is until the next call of ResolveAbsolutePath.
//!         Copy this return value immediatly after the call to avoid most issues
const char* ResolveAbsolutePath_cStr(const char* path)
{
  static std::string storage = std::string{ path };
  storage = ResolveAbsolutePath(storage);
  return storage.c_str();
}

std::vector<std::string> ListFiles(const std::string& dir, const std::string& regex, bool recurse)
{
  std::vector<std::string> files;
  ListFiles(dir, files, regex, recurse);
  return files;
}

void MakeDirectory(const std::string& dir)
{
  MKDIR(dir.c_str());
}

void DeleteDirectory(const std::string& dir, bool bDeleteSubdirectories)
{
  DIR* d;
  dirent* ent;
  std::string filename;
  if ((d = opendir(dir.c_str())) != nullptr) {
    while ((ent = readdir(d)) != nullptr) {
      size_t nameLength = strlen(ent->d_name);

      if (ent->d_name[0] == '.' && ((nameLength == 1) || (nameLength == 2 && ent->d_name[1] == '.')))
        continue;
      filename = dir;
      filename += "/";
      filename += ent->d_name;

      if (!IsDirectory(ent)) {
        std::remove(filename.c_str());
      } else {
        DeleteDirectory(filename, bDeleteSubdirectories);
      }
    }
  }
  RMDIR(dir.c_str());
}

void SetCurrentWorkingDirectory(std::string working_dir)
{
  g_working_dir = std::move(working_dir);
}

void SetCurrentWorkingDirectory(const char* working_dir)
{
  g_working_dir = working_dir;
}

bool TestLastDirName(std::string path, std::string dirname)
{
  filesystem::path p{ std::move(path) };
  if (!filesystem::is_directory(p)) {
    p = p.parent_path();
  }
  return p.filename().string() == dirname;
}
bool TestFirstDirName(std::string path, std::string dirname)
{
  filesystem::path p{ std::move(path) };
  p = p.make_normal();
  if (!p.is_absolute()) {
    if (p.begin() != p.end()) {
      auto itr = p.begin();
        return *itr == dirname;
    } else {

    }
  }
  return false;
}

bool TestLastDirName(const char* path, const char* dirname)
{
  return TestLastDirName(std::string{ path }, std::string{ dirname });
}

bool TestFirstDirName(const char* path, const char* dirname)
{
  return TestFirstDirName(std::string{ path }, std::string{ dirname });
}

std::string GetCurrentWorkingDirectory()
{
  return g_working_dir;
}

const char* GetCurrentWorkingDirectory_cStr()
{
  return g_working_dir.c_str();
}

BIOGEARS_API std::recursive_mutex g_fileSystemMutex;

ScopedFileSystemLock::ScopedFileSystemLock()
{
  g_fileSystemMutex.lock();
}

ScopedFileSystemLock::~ScopedFileSystemLock()
{
  g_fileSystemMutex.unlock();
}

bool IsDirectory(struct ::dirent* ent)
{
#if defined(__MINGW64_VERSION_MAJOR) || defined(__gnu_linux__)
  struct stat info;
  if (stat(ent->d_name, &info) == 0) {
    return S_ISDIR(info.st_mode);
  }
  return 0;
#else
  return ent->d_type == DT_DIR;
#endif
}
}
