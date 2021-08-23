//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.apache.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------

#include <biogears/filesystem/path.h>

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#if defined(ANDROID)
#include <android/log.h>
#endif

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <ShlObj.h>
#include <shellapi.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif
#include <sys/stat.h>
#if defined(__linux)
#include <linux/limits.h>
#endif

namespace biogears {
namespace filesystem {

  struct path::Implementation {
  public:
    mutable bool dirty = true;
    mutable std::string string_cashe;

#if defined(_WIN32)
    mutable std::wstring wstring_cashe;
#endif

    path_type type;
    value_type path;
    bool absolute;
  };
#if defined(_WIN32)
  static constexpr size_t MAX_PATH_WINDOWS = 32767;
#endif
  static constexpr size_t MAX_PATH_WINDOWS_LEGACY = 260;

  //!
  //! Path Constructors
  //!
  path::path()
    : _impl(std::make_unique<Implementation>())
  {
    _impl->type = default_path;
    _impl->absolute = false;
  }
  //-------------------------------------------------------------------------------
  path::path(const path& path)
    : _impl(std::make_unique<Implementation>())
  {
    _impl->type = path._impl->type;
    _impl->path = path._impl->path;
    _impl->absolute = path._impl->absolute;
    _impl->dirty = path._impl->dirty;
    _impl->string_cashe = path._impl->string_cashe;
#if defined(_WIN32)
    _impl->wstring_cashe = path._impl->wstring_cashe;
#endif
  }
  //-------------------------------------------------------------------------------
  path::path(path&& path)
    : _impl(std::make_unique<Implementation>())
  {
    _impl->type = (std::move(path._impl->type));
    _impl->path = (std::move(path._impl->path));
    _impl->absolute = (std::move(path._impl->absolute));
    _impl->dirty = (std::move(path._impl->dirty));
    _impl->string_cashe = (std::move(path._impl->string_cashe));
#if defined(_WIN32)
    _impl->wstring_cashe = (std::move(path._impl->wstring_cashe));
#endif
  }
  //-------------------------------------------------------------------------------
  path::path(const char* string)
    : _impl(std::make_unique<Implementation>())
  {
    if (string) {
      set(string);
    } else {
      set("");
    }
  }
  //-------------------------------------------------------------------------------
  path::path(const std::string& string)
    : _impl(std::make_unique<Implementation>())
  {
    set(string);
  }
  //-------------------------------------------------------------------------------
#if defined(_WIN32)
  path::path(const std::wstring& wstring)
    : _impl(std::make_unique<Implementation>())
  {
    set(wstring);
  }
  //-------------------------------------------------------------------------------
  path::path(const wchar_t* wstring)
    : _impl(std::make_unique<Implementation>())
  {
    if (wstring) {
      set(wstring);
    } else {
      set("");
    }
  }
  //-------------------------------------------------------------------------------
#endif
  path::~path()
  {
    _impl.reset();
  }
  //!
  //! Path Iterators
  //!
  auto path::begin() -> iterator
  {
    _impl->dirty = true;
    return _impl->path.begin();
  }
  //-------------------------------------------------------------------------------
  auto path::end() -> iterator
  {
    _impl->dirty = true;
    return _impl->path.end();
  }
  //-------------------------------------------------------------------------------
  auto path::back() const -> std::string
  {
    return (_impl->path.size()) ? _impl->path.back() : "";
  }
  //-------------------------------------------------------------------------------
  auto path::begin() const -> const_iterator
  {
    _impl->dirty = true;
    return _impl->path.begin();
  }
  //-------------------------------------------------------------------------------
  auto path::end() const -> const_iterator
  {
    _impl->dirty = true;
    return _impl->path.end();
  }
  //-------------------------------------------------------------------------------
  //!
  //! Manipulation
  //!
  path path::make_absolute() const
  {
#if !defined(_WIN32)
    char temp[PATH_MAX];
    if (realpath(ToString().c_str(), temp) == NULL)
      throw std::runtime_error("Internal error in realpath(): " + std::string(strerror(errno)));
    return path(temp);
#else
    std::wstring value = ToWString(), out(MAX_PATH_WINDOWS, '\0');
    value = (value.empty()) ? L"." : value;

    DWORD length = GetFullPathNameW(value.c_str(), MAX_PATH_WINDOWS, &out[0], NULL);
    if (length == 0)
      throw std::runtime_error("Internal error in realpath(): " + std::to_string(GetLastError()));
    return path(out.substr(0, length));
#endif
  }
  //-------------------------------------------------------------------------------
  path path::make_normal() const
  {
    path working_path;
    working_path._impl->absolute = _impl->absolute;
    working_path._impl->type = _impl->type;
    for (auto& segment : _impl->path) {
      if (segment.empty()) {

      } else if (segment == ".") {

      } else if (segment == "..") {
        if (working_path.depth() > 0 && working_path.back() != "..") {
          working_path._impl->path.pop_back();
        } else {
          working_path / "..";
        }
      } else {
        working_path /= segment;
      }
    }
    return working_path;
  }
  //-------------------------------------------------------------------------------
  void path::set(const std::string& str, path_type type)
  {
    _impl->dirty = true;
    _impl->type = type;
    if (str.empty() || std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isspace(c); })) {
      _impl->absolute = false;
    } else {

      std::string windows_lpath_prefix = R"(^\\\\\?\\)";
      auto tmp = str;
      if (tmp.length() >= windows_lpath_prefix.length()
          && std::mismatch(std::begin(windows_lpath_prefix), std::end(windows_lpath_prefix), std::begin(tmp)).first == std::end(windows_lpath_prefix)) {
        tmp.erase(0, 4);
      }
      _impl->path = tokenize(tmp, R"(/\)");
      std::regex absolute_regex { "^[a-zA-Z]:|^/" };
      _impl->absolute = std::regex_search(tmp, absolute_regex);
    }
  }
  //-------------------------------------------------------------------------------
  path::path_type path::mode() const
  {
    return _impl->type;
  }
  //-------------------------------------------------------------------------------
  void path::mode(path_type type)
  {
    _impl->type = type;
    _impl->dirty = true;
  }
  //-------------------------------------------------------------------------------
  //!
  //! Properties
  //!
  size_t path::depth() const
  {
    return _impl->path.size();
  }
  size_t path::length() const
  {
    if (_impl->dirty) {
      generate_string_cashe();
    }
    return _impl->string_cashe.size();
  }
  //-------------------------------------------------------------------------------
  size_t path::file_size() const
  {
#if defined(_WIN32)
    struct _stati64 sb;
    if (_wstati64(ToWString().c_str(), &sb) != 0)
      throw std::runtime_error("path::file_size(): cannot stat file \"" + ToString() + "\"!");
#else
    struct stat sb;
    if (stat(ToString().c_str(), &sb) != 0)
      throw std::runtime_error("path::file_size(): cannot stat file \"" + ToString() + "\"!");
#endif
    return (size_t)sb.st_size;
  }
  //-------------------------------------------------------------------------------
  bool path::empty() const { return _impl->path.empty(); }
  //-------------------------------------------------------------------------------
  bool path::exists() const
  {
#if defined(_WIN32)
    return GetFileAttributesW(ToWString().c_str()) != INVALID_FILE_ATTRIBUTES;
#else
    struct stat sb;
    return stat(ToString().c_str(), &sb) == 0;
#endif
  }
  //-------------------------------------------------------------------------------
  bool path::is_directory() const
  {
#if defined(_WIN32)
    DWORD result = GetFileAttributesW(ToWString().c_str());
    if (result == INVALID_FILE_ATTRIBUTES)
      return false;
    return (result & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
    struct stat sb;
    if (stat(ToString().c_str(), &sb))
      return false;
    return S_ISDIR(sb.st_mode);
#endif
  }
  //-------------------------------------------------------------------------------
  bool path::is_file() const
  {
#if defined(_WIN32)
    DWORD attr = GetFileAttributesW(ToWString().c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
    struct stat sb;
    if (stat(ToString().c_str(), &sb))
      return false;
    return S_ISREG(sb.st_mode);
#endif
  }
  //-------------------------------------------------------------------------------
  bool path::is_absolute() const { return _impl->absolute; }
  //-------------------------------------------------------------------------------

  //!
  //! Path Components
  //!
  std::string path::extension() const
  {
    std::string name = filename();
    size_t pos = name.find_last_of(".");
    if (pos == std::string::npos)
      return "";
    return name.substr(pos + 1);
  }
  //-------------------------------------------------------------------------------
  path path::dirname() const
  {
    if (is_directory()) {
      return *this;
    } else {
      return parent_path();
    }
  }
  //-------------------------------------------------------------------------------
  std::string path::basename() const
  {
    if (empty())
      return "";
    return _impl->path[_impl->path.size() - 1];
  }
  //-------------------------------------------------------------------------------
  std::string path::filename() const
  {
    if (empty())
      return "";
    return _impl->path[_impl->path.size() - 1];
  }
  //-------------------------------------------------------------------------------
  path path::parent_path() const
  {
    path result;
    result._impl->absolute = _impl->absolute;

    if (_impl->path.empty()) {
      if (!_impl->absolute)
        result._impl->path.push_back("..");
    } else {
      size_t until = _impl->path.size() - 1;
      for (size_t i = 0; i < until; ++i)
        result._impl->path.push_back(_impl->path[i]);
    }
    return result;
  }
  //-------------------------------------------------------------------------------

  //!
  //! Operator Overloads
  //!
  path& path::operator/=(const path& other)
  {
    _impl->dirty = true;
    *this = *this / other;
    return *this;
  }
  //-------------------------------------------------------------------------------
  path path::operator/(const path& other) const
  {
    if (other._impl->absolute) {
      if (_impl->path.empty()) {
        return other;
      } else {
        throw std::runtime_error("path::operator/(): expected a relative path!");
      }
    }
    path result(*this);

    for (size_t i = 0; i < other._impl->path.size(); ++i)
      result._impl->path.push_back(other._impl->path[i]);

    return result;
  }
  //-------------------------------------------------------------------------------
  path& path::operator=(const path& path)
  {
    _impl->type = path._impl->type;
    _impl->path = path._impl->path;
    _impl->absolute = path._impl->absolute;
    _impl->dirty = path._impl->dirty;
    _impl->string_cashe = path._impl->string_cashe;
#if defined(_WIN32)
    _impl->wstring_cashe = path._impl->wstring_cashe;
#endif
    return *this;
  }
  //-------------------------------------------------------------------------------
  path& path::operator=(path&& path)
  {
    if (this != &path) {
      _impl->type = path._impl->type;
      _impl->path = std::move(path._impl->path);
      _impl->absolute = path._impl->absolute;
      _impl->dirty = path._impl->dirty;
      _impl->string_cashe = std::move(path._impl->string_cashe);
#if defined(_WIN32)
      _impl->wstring_cashe = std::move(path._impl->wstring_cashe);
#endif
    }
    return *this;
  }
  //-------------------------------------------------------------------------------
  bool path::operator==(const path& p) const
  {
    return p._impl->path == _impl->path;
  }
  //-------------------------------------------------------------------------------
  bool path::operator!=(const path& p) const { return p._impl->path != _impl->path; }

  //!
  //! Stringification
  //!
  char const* path::c_str() const
  {
    if (_impl->dirty) {
      generate_string_cashe();
    }
    return _impl->string_cashe.c_str();
  }
  //-------------------------------------------------------------------------------
  bool path::operator<=(const path& rhs) const
  {
    path lhs_n = make_normal();
    path rhs_n = rhs.make_normal();

    if (lhs_n.depth() < rhs_n.depth()) {
      for (auto ii = 0; ii < lhs_n.depth(); ++ii) {
        if (lhs_n._impl->path[ii] != rhs_n._impl->path[ii]) {
          return false;
        }
      }
      return true;
    }

    return false;
  }
  //-------------------------------------------------------------------------------
  std::string path::ToString() const
  {
    if (_impl->dirty) {
      generate_string_cashe();
    }
    return _impl->string_cashe;
  }
  //-------------------------------------------------------------------------------
  std::string path::ToString(path_type type) const
  {
    char const* path_seperator = (posix_path == type) ? "/" : "\\";

    std::ostringstream oss;
    if (_impl->absolute) {
      if (type == windows_path) {
        size_t length = 0;
        for (size_t i = 0; i < _impl->path.size(); ++i)
          // No special case for the last segment to count the NULL character
          length += _impl->path[i].length() + 1;
        // Windows requires a \\?\ prefix to handle paths longer than MAX_PATH
        // (including their null character). NOTE: relative paths >MAX_PATH are
        // not supported at all in Windows.
        if (length > MAX_PATH_WINDOWS_LEGACY)
          oss << R"(\\?\)";
      }
      if (_impl->path.size() > 0 && _impl->path[0].size() > 1 && _impl->path[0][1] == ':') {
        oss << _impl->path[0];
      } else if (_impl->path.size() != 0) {
        oss << path_seperator << _impl->path[0];
      }
    } else {
      if (_impl->path.size() != 0) {
        oss << _impl->path[0];
      }
    }

    for (size_t i = 1; i < _impl->path.size(); ++i) {
      if (type == posix_path) {
        oss << '/';
      } else {
        oss << '\\';
      }
      oss << _impl->path[i];
    }
    return oss.str();
  }
  //-------------------------------------------------------------------------------
  std::ostream& operator<<(std::ostream& os, const path& path)
  {
    os << path.ToString();
    return os;
  }
  //-------------------------------------------------------------------------------
  path::operator std::string() const
  {
    return this->ToString(posix_path);
  }
  //-------------------------------------------------------------------------------
  void path::generate_string_cashe() const
  {
    _impl->string_cashe = ToString(_impl->type);
#if defined(_WIN32)
    int size = MultiByteToWideChar(CP_UTF8, 0, &_impl->string_cashe[0], (int)_impl->string_cashe.size(), NULL, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, &_impl->string_cashe[0], (int)_impl->string_cashe.size(), &result[0], size);
    _impl->wstring_cashe = result;
#endif
    _impl->dirty = false;
  }
#if defined(_WIN32)
  std::wstring path::ToWString() const
  {
    if (_impl->dirty) {
      generate_string_cashe();
    }
    return _impl->wstring_cashe;
  }
  //-------------------------------------------------------------------------------
  std::wstring path::ToWString(path_type type) const
  {
    std::string string_rep = ToString(type);
    int size = MultiByteToWideChar(CP_UTF8, 0, &string_rep[0], (int)string_rep.size(), NULL, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, &string_rep[0], (int)string_rep.size(), &result[0], size);
    return result;
  }
  //-------------------------------------------------------------------------------
  void path::set(const std::wstring& wstring, path_type type)
  {
    std::string string;
    if (!wstring.empty()) {
      int size = WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(),
                                     NULL, 0, NULL, NULL);
      string.resize(size, 0);
      WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(),
                          &string[0], size, NULL, NULL);
    }
    set(string, type);
  }
  //-------------------------------------------------------------------------------
  path& path::operator=(const std::wstring& str)
  {
    set(str);
    return *this;
  }
  path::operator std::wstring() const
  {
    return this->ToWString();
  }
  //-------------------------------------------------------------------------------
#endif
  std::vector<std::string> path::tokenize(const std::string& string, const std::string& delim)
  {
    std::string::size_type lastPos = 0, pos = string.find_first_of(delim, lastPos);
    std::vector<std::string> tokens;

    while (lastPos != std::string::npos) {
      if (pos != lastPos)
        tokens.push_back(string.substr(lastPos, pos - lastPos));
      lastPos = pos;
      if (lastPos == std::string::npos || lastPos + 1 == string.length())
        break;
      pos = string.find_first_of(delim, ++lastPos);
    }

    return tokens;
  }
  //-------------------------------------------------------------------------------

  //!
  //! Quick Functions
  //!
  path cwd()
  {
#if !defined(_WIN32)
    char temp[PATH_MAX];
    if (::getcwd(temp, PATH_MAX) == NULL)
      throw std::runtime_error("Internal error in getcwd(): " + std::string(strerror(errno)));
    return path(temp);
#else
    std::wstring temp(MAX_PATH_WINDOWS, '\0');
    if (!_wgetcwd(&temp[0], MAX_PATH_WINDOWS))
      throw std::runtime_error("Internal error in getcwd(): " + std::to_string(GetLastError()));
    return path(temp.c_str());
#endif
  }
  //-------------------------------------------------------------------------------
  bool rm(const path& filepath)
  {
#if !defined(_WIN32)
#ifndef ANDROID
    return remove(filepath.ToString().c_str()) == 0;
#endif
    return false;
#else
    return DeleteFileW(filepath.ToWString().c_str()) != 0;
#endif
  }
  //-------------------------------------------------------------------------------
  bool rm(const std::string& file)
  {
#if !defined(_WIN32)
#ifndef ANDROID
    return remove(file.c_str()) == 0;
#endif
    return false;
#else
    path pp { file };
    return DeleteFileW(pp.ToWString().c_str()) != 0;
#endif
  }
  //-------------------------------------------------------------------------------
  //!
  //! std::remove will actually remove files or directories on posix systems
  //! We could return an error if the path isn't the right time, but it seems like
  //! an unncessary burden Maybe if we ever expand the functions to take operation modes
  bool rmdir(const path& directory_path)
  {
#if !defined(_WIN32)
#ifndef ANDROID
    return remove(directory_path.ToString().c_str()) == 0;
#endif
    return false;
#else
    return RemoveDirectoryW(directory_path.ToWString().c_str()) != 0;
#endif
  }
  //-------------------------------------------------------------------------------
  bool rmdir(const std::string& directory)
  {

#if !defined(_WIN32)
#ifndef ANDROID
    return remove(directory.c_str()) == 0;
#endif
    return false;
#else
    path p { directory };
    return RemoveDirectoryW(p.ToWString().c_str()) != 0;
#endif
  }
  bool exists(const std::string& name)
  {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
  }
  //-------------------------------------------------------------------------------
  //!
  //! rm -rf directory
  //!
  bool rmdirs(const std::string& directory)
  {
#if defined(_WIN32)
    std::string path_list = directory + '\0';
    SHFILEOPSTRUCT shfo = {
      NULL,
      FO_DELETE,
      path_list.c_str(), //<! This is a list of strings so the last element is double \0 terminated.
      NULL,
      FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION,
      FALSE,
      NULL,
      NULL
    };

    return 0 == SHFileOperation(&shfo);
#else

    std::function<int(const char*)> rmrf = [&rmrf](const char* dirname) {
      DIR* dir;
      struct dirent* entry;
      char path[PATH_MAX];

      dir = opendir(dirname);
      if (dir == NULL) {
        perror("Error opendir()");
        return 0;
      }

      while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
          snprintf(path, (size_t)PATH_MAX, "%s/%s", dirname, entry->d_name);
          if (entry->d_type == DT_DIR) {
            rmrf(path);
          }
          printf("(not really) Deleting: %s\n", path);
        }
      }
      closedir(dir);
      printf("(not really) Deleting: %s\n", dirname);
      return 1;
    };
    return rmrf(directory.c_str());
#endif
  }
  bool rmdirs(const path& p)
  {
#if defined(_WIN32)
    std::string path_list = p.ToString() + '\0';
    SHFILEOPSTRUCT shfo = {
      NULL,
      FO_DELETE,
      path_list.c_str(), //<! This is a list of strings so the last element is double \0 terminated.
      NULL,
      FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION,
      FALSE,
      NULL,
      NULL
    };

    return SHFileOperation(&shfo) == 0;
#else

    std::function<int(const char*)> rmrf = [&rmrf](const char* dirname) {
      DIR* dir;
      struct dirent* entry;
      char path[PATH_MAX];

      dir = opendir(dirname);
      if (dir == NULL) {
        perror("Error opendir()");
        return 0;
      }

      while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
          snprintf(path, (size_t)PATH_MAX, "%s/%s", dirname, entry->d_name);
          if (entry->d_type == DT_DIR) {
            rmrf(path);
          }
          printf("(not really) Deleting: %s\n", path);
        }
      }
      closedir(dir);
      printf("(not really) Deleting: %s\n", dirname);
      return 1;
    };
    return rmrf(p.c_str());
#endif
  }
  //-------------------------------------------------------------------------------
  bool create_directory(const path& p)
  {
#if defined(_WIN32)
    return CreateDirectoryW(p.ToWString().c_str(), NULL) != 0;
#else
    return mkdir(p.ToString().c_str(), S_IRWXU) == 0;
#endif
  }
  //-------------------------------------------------------------------------------
  bool create_directories(const path& p)
  {
#if defined(_WIN32)
    return SHCreateDirectory(nullptr, p.make_absolute().ToWString(path::windows_path).c_str()) == ERROR_SUCCESS;
#else
    if (create_directory(p.ToString().c_str()))
      return true;

    if (p.empty())
      return false;

    bool success = true;
    path tmp;
    for (auto segment : p) {
      tmp /= segment;
      auto result = mkdir(tmp.ToString(path::posix_path).c_str(), S_IRWXU);
      success = success && (result == 0 || result == EEXIST);
      if (!success) {
        return success;
      }
    }
    return success;
#endif
  }
  //-------------------------------------------------------------------------------
  //-------------------------------------------------------------------------------
} // namespace filesystem
} //namespace biogears
