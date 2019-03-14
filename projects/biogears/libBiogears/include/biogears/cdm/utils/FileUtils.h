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

#pragma once

#include <biogears/cdm/CommonDataModel.h>

#include <mutex>
#include <string>
#include <vector>

#if defined(_MSC_VER) || defined(__MINGW64_VERSION_MAJOR)

#include <windows.h>

#else

#include <unistd.h>

#if defined(__clang__) && !defined(ANDROID)
#include <sys/syslimits.h>
#endif

#if defined(__gnu_linux__)
#include <cstring>
#endif

#endif

struct dirent;

namespace biogears {
BIOGEARS_API bool CreateFilePath(const std::string&);
BIOGEARS_API bool CreateFilePath(const char*);
BIOGEARS_API std::string Replace(const std::string& original, const std::string& replace, const std::string& withThis);
BIOGEARS_API void ListFiles(const std::string& dir, std::vector<std::string>& files, const std::string& mask = "", bool recurse = true);
BIOGEARS_API std::vector<std::string> ListFiles(const std::string& dir, const std::string& regex, bool recurse = true);

BIOGEARS_API bool IsAbsolutePath(const std::string& path);
BIOGEARS_API bool IsAbsolutePath(const char* path);
BIOGEARS_API std::string ResolveAbsolutePath(const std::string& path);
BIOGEARS_API const char* ResolveAbsolutePath_cStr(const char* path);
BIOGEARS_API void SetCurrentWorkingDirectory(std::string working_dir);
BIOGEARS_API void SetCurrentWorkingDirectory(const char* working_dir);
BIOGEARS_API bool TestLastDirName(std::string path, std::string dirname);
BIOGEARS_API bool TestFirstDirName(std::string path, std::string dirname);
BIOGEARS_API bool TestLastDirName(const char* path, const char* dirname);
BIOGEARS_API bool TestFirstDirName(const char* path, const char* dirname);

BIOGEARS_API std::string GetCurrentWorkingDirectory();
BIOGEARS_API const char* GetCurrentWorkingDirectory_cStr();

BIOGEARS_API void DeleteDirectory(const std::string& dir, bool bDeleteSubdirectories = true);
BIOGEARS_API void MakeDirectory(const std::string& dir);

BIOGEARS_API extern std::recursive_mutex g_fileSystemMutex;

class BIOGEARS_API ScopedFileSystemLock {
public:
  ScopedFileSystemLock();
  ~ScopedFileSystemLock();

  ScopedFileSystemLock(const ScopedFileSystemLock& other) = delete;
  ScopedFileSystemLock& operator=(const ScopedFileSystemLock& other) = delete;

  ScopedFileSystemLock(ScopedFileSystemLock&& other) = delete;
  ScopedFileSystemLock& operator=(ScopedFileSystemLock&& other) = delete;
};

bool BIOGEARS_API IsDirectory(dirent* ent);
}
