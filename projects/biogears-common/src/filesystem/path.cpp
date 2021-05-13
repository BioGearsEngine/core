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
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <ShlObj.h>
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <sys/stat.h>
#if defined(__linux)
#include <linux/limits.h>
#endif

namespace biogears {
namespace filesystem {

#if defined(_WIN32)
  static constexpr size_t MAX_PATH_WINDOWS = 32767;
#endif
  static constexpr size_t MAX_PATH_WINDOWS_LEGACY = 260;

  //!
  //! Path Constructors
  //!
  path::path()
    : m_type(default_path)
    , m_absolute(false)
  {
  }
  //-------------------------------------------------------------------------------
  path::path(const path& path)
    : m_type(path.m_type)
    , m_path(path.m_path)
    , m_absolute(path.m_absolute)
    , m_dirty(path.m_dirty)
    , m_string_cashe(path.m_string_cashe)
#if defined(_WIN32)
    , m_wstring_cashe(path.m_wstring_cashe)
#endif
  {
  }
  //-------------------------------------------------------------------------------
  path::path(path&& path)
    : m_type(std::move(path.m_type))
    , m_path(std::move(path.m_path))
    , m_absolute(std::move(path.m_absolute))
    , m_dirty(std::move(path.m_dirty))
    , m_string_cashe(std::move(path.m_string_cashe))
#if defined(_WIN32)
    , m_wstring_cashe(std::move(path.m_wstring_cashe))
#endif
  {
  }
  //-------------------------------------------------------------------------------
  path::path(const char* string)
  {
    set(string);
  }
  //-------------------------------------------------------------------------------
  path::path(const std::string& string)
  {
    set(string);
  }
  //-------------------------------------------------------------------------------
#if defined(_WIN32)
  path::path(const std::wstring& wstring)
  {
    set(wstring);
  }
  //-------------------------------------------------------------------------------
  path::path(const wchar_t* wstring)
  {
    set(wstring);
  }
  //-------------------------------------------------------------------------------
#endif

  //!
  //! Path Iterators
  //!
  auto path::begin() -> iterator
  {
    m_dirty = true;
    return m_path.begin();
  }
  //-------------------------------------------------------------------------------
  auto path::end() -> iterator
  {
    m_dirty = true;
    return m_path.end();
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
    working_path.m_absolute = m_absolute;
    working_path.m_type = m_type;
    for (auto& segment : m_path) {
      if (segment.empty()) {

      } else if (segment == ".") {

      } else if (segment == "..") {
        working_path = working_path.parent_path();
      } else {
        working_path /= segment;
      }
    }
    return working_path;
  }
  //-------------------------------------------------------------------------------
  void path::set(const std::string& str, path_type type)
  {
    m_dirty = true;
    m_type = type;
    if (str.empty() || std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isspace(c); })) {
      m_absolute = false;
    } else if (type == windows_path) {
      std::string tmp = str;

      // Long windows paths (sometimes) begin with the prefix \\?\. It should only
      // be used when the path is >MAX_PATH characters long, so we remove it
      // for convenience and add it back (if necessary) in ToString()/ToWString().
      static const std::string PREFIX = R"(\\?\)";
      if (tmp.length() >= PREFIX.length()
          && std::mismatch(std::begin(PREFIX), std::end(PREFIX), std::begin(tmp)).first == std::end(PREFIX)) {
        tmp.erase(0, 4);
      }
      m_path = tokenize(tmp, R"(/\)");
      m_absolute = tmp.size() >= 2 && std::isalpha(tmp[0]) && tmp[1] == ':';
    } else {
      m_path = tokenize(str, R"(/\)");
      m_absolute = !str.empty() && str[0] == '/';
    }
  }
  //-------------------------------------------------------------------------------
  path::path_type path::mode()
  {
    return m_type;
  }
  //-------------------------------------------------------------------------------
  void path::mode(path_type type)
  {
    m_type = type;
  }
  //-------------------------------------------------------------------------------
  //!
  //! Properties
  //!
  size_t path::length() const
  {
    return m_path.size();
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
  bool path::empty() const { return m_path.empty(); }
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
  bool path::is_absolute() const { return m_absolute; }
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
    return m_path[m_path.size() - 1];
  }
  //-------------------------------------------------------------------------------
  std::string path::filename() const
  {
    if (empty())
      return "";
    return m_path[m_path.size() - 1];
  }
  //-------------------------------------------------------------------------------
  path path::parent_path() const
  {
    path result;
    result.m_absolute = m_absolute;

    if (m_path.empty()) {
      if (!m_absolute)
        result.m_path.push_back("..");
    } else {
      size_t until = m_path.size() - 1;
      for (size_t i = 0; i < until; ++i)
        result.m_path.push_back(m_path[i]);
    }
    return result;
  }
  //-------------------------------------------------------------------------------

  //!
  //! Operator Overloads
  //!
  path& path::operator/=(const path& other)
  {
    m_dirty = true;
    *this = *this / other;
    return *this;
  }
  //-------------------------------------------------------------------------------
  path path::operator/(const path& other) const
  {
    if (other.m_absolute) {
      if (m_path.empty()) {
        return other;
      } else {
        throw std::runtime_error("path::operator/(): expected a relative path!");
      }
    }
    path result(*this);

    for (size_t i = 0; i < other.m_path.size(); ++i)
      result.m_path.push_back(other.m_path[i]);

    return result;
  }
  //-------------------------------------------------------------------------------
  path& path::operator=(const path& path)
  {
    m_type = path.m_type;
    m_path = path.m_path;
    m_absolute = path.m_absolute;
    m_dirty = path.m_dirty;
    m_string_cashe = path.m_string_cashe;
#if defined(_WIN32)
    m_wstring_cashe = path.m_wstring_cashe;
#endif
    return *this;
  }
  //-------------------------------------------------------------------------------
  path& path::operator=(path&& path)
  {
    if (this != &path) {
      m_type = path.m_type;
      m_path = std::move(path.m_path);
      m_absolute = path.m_absolute;
      m_dirty = path.m_dirty;
      m_string_cashe = std::move(path.m_string_cashe);
#if defined(_WIN32)
      m_wstring_cashe = std::move(path.m_wstring_cashe);
#endif
    }
    return *this;
  }
  //-------------------------------------------------------------------------------
  bool path::operator==(const path& p) const
  {
    return p.m_path == m_path;
  }
  //-------------------------------------------------------------------------------
  bool path::operator!=(const path& p) const { return p.m_path != m_path; }

  //!
  //! Stringification
  //!
  char const* path::c_str() const
  {
    if (m_dirty) {
      generate_string_cashe();
    }
    return m_string_cashe.c_str();
  }
  //-------------------------------------------------------------------------------
  std::string path::ToString() const
  {
    if (m_dirty) {
      generate_string_cashe();
    }
    return m_string_cashe;
  }
  //-------------------------------------------------------------------------------
  std::string path::ToString(path_type type) const
  {
    std::ostringstream oss;
    if (m_absolute) {
      if (type == posix_path)
        oss << "/";
      else {
        size_t length = 0;
        for (size_t i = 0; i < m_path.size(); ++i)
          // No special case for the last segment to count the NULL character
          length += m_path[i].length() + 1;
        // Windows requires a \\?\ prefix to handle paths longer than MAX_PATH
        // (including their null character). NOTE: relative paths >MAX_PATH are
        // not supported at all in Windows.
        if (length > MAX_PATH_WINDOWS_LEGACY)
          oss << R"(\\?\)";
      }
    }

    for (size_t i = 0; i < m_path.size(); ++i) {
      oss << m_path[i];
      if (i + 1 < m_path.size()) {
        if (type == posix_path)
          oss << '/';
        else
          oss << '\\';
      }
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
    m_string_cashe = ToString(m_type);
#if defined(_WIN32)
    int size = MultiByteToWideChar(CP_UTF8, 0, &m_string_cashe[0], (int)m_string_cashe.size(), NULL, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, &m_string_cashe[0], (int)m_string_cashe.size(), &result[0], size);
    m_wstring_cashe = result;
#endif
    m_dirty = false;
  }
#if defined(_WIN32)
  std::wstring path::ToWString() const
  {
    if (m_dirty) {
      generate_string_cashe();
    }
    return m_wstring_cashe;
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
    return std::remove(filepath.ToString().c_str()) == 0;
#else
    return DeleteFileW(filepath.ToWString().c_str()) != 0;
#endif
  }
  //-------------------------------------------------------------------------------
  bool rm(const std::string& file)
  {
#if !defined(_WIN32)
    return std::remove(file.c_str()) == 0;
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
    return std::remove(directory_path.ToString().c_str()) == 0;
#else
    return RemoveDirectoryW(directory_path.ToWString().c_str()) != 0;
#endif
  }
  //-------------------------------------------------------------------------------
  bool rmdir(const std::string& directory)
  {
    
#if !defined(_WIN32)
    return std::remove(directory.c_str()) == 0;
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
    return SHCreateDirectory(nullptr, p.make_absolute().ToWString().c_str()) == ERROR_SUCCESS;
#else
    if (create_directory(p.ToString().c_str()))
      return true;

    if (p.empty())
      return false;

    if (errno == ENOENT) {
      if (create_directory(p.parent_path()))
        return mkdir(p.ToString().c_str(), S_IRWXU) == 0;
      else
        return false;
    }
    return false;
#endif
  }
  //-------------------------------------------------------------------------------
  //-------------------------------------------------------------------------------
} // namespace filesystem
} //namespace biogears
