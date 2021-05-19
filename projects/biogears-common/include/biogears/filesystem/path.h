#ifndef BIOGEARS_COMMON_FILESYSTEM_PATH_H
#define BIOGEARS_COMMON_FILESYSTEM_PATH_H
/*
    path.h -- A simple class for manipulating paths on Linux/Windows/Mac OS

Copyright (c) 2016 Wenzel Jakob <wenzel.jakob@epfl.ch>, All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches, or
upgrades to the features, functionality or performance of the source code
("Enhancements") to anyone; however, if you choose to make your Enhancements
available either publicly, or directly to the author of this software, without
imposing a separate written license agreement for such Enhancements, then you
hereby grant the following license: a non-exclusive, royalty-free perpetual
license to install, use, modify, prepare derivative works, incorporate into
other computer software, distribute, and sublicense such enhancements or
derivative works thereof, in binary and source code form.
*/

//This file has been modified from its original version as part of the biogears project

#include <biogears/common-exports.h>

#include <string>
#include <vector>

namespace biogears {
namespace filesystem {

  /**
 * \brief Simple class for manipulating paths on Linux/Windows/Mac OS
 *
 * This class is just a temporary workaround to avoid the heavy boost
 * dependency until boost::filesystem is integrated into the standard template
 * library at some point in the future.
 */
  class BIOGEARS_COMMON_PUBLIC_API path {
  public:
    using value_type = std::vector<std::string>;
    using iterator = value_type::iterator;
    using const_iterator = value_type::const_iterator;
    using reference = value_type::reference;
    using const_reference = value_type::const_reference;

    enum path_type {
      windows_path = 0,
      posix_path = 1,
      default_path = posix_path,
#if defined(_WIN32)
      native_path = windows_path
#else
      native_path = posix_path
#endif
    };

    //!
    //! Constructors
    //!
    path();
    path(const path& path);
    path(path&& path);
    path(const char* string);
    path(const std::string& string);
#if defined(_WIN32)
    path(const std::wstring& wstring);
    path(const wchar_t* wstring);
#endif
    //!
    //! Path Iteration
    //!
    auto begin() -> iterator;
    auto end() -> iterator;
    auto begin() const -> const_iterator;
    auto end() const -> const_iterator;
    auto back() const -> std::string; 
    //!
    //! Manipulation
    //!
    path make_absolute() const;
    path make_normal() const;
    void set(const std::string& str, path_type type = default_path);
    path_type mode() const;
    void mode(path_type);
    //!
    //! Properties
    //!
    size_t length() const;
    size_t file_size() const;

    bool empty() const;
    bool exists() const;
    bool is_directory() const;
    bool is_file() const;
    bool is_absolute() const;

    //!
    //! Path Components
    //!
    std::string extension() const; //< Last Extension of the basename()
    std::string basename() const; //< Value of last path segment
    std::string filename() const; //< same as basename()

    path dirname() const; //<  Returns all segments if is_directory else return parent_path
    path parent_path() const; //<  Returns the path suffixed by ..; For absulte paths this is resolved

    //!
    //! Operator Overloads
    //!
    path& operator/=(const path& other);
    path operator/(const path& other) const;
    path& operator=(const path& path);
    path& operator=(path&& path);
    bool operator==(const path& p) const;
    bool operator!=(const path& p) const;

    //! Is the lhs a parent path of the rhs
    bool operator<=(const path& p) const;
    
    //!
    //! Stringification
    //!
    char const* c_str() const; //< Calculates the string cashe and returns its c_str(). Valid until next non const function call
    std::string ToString() const; //< Cashes the string of m_type
    std::string ToString(path_type type) const; //< Produces a string of type and does not cashe results
    operator std::string() const; //< Cashes the string of m_type

#if defined(_WIN32)
    std::wstring ToWString() const; //< Cashes the string of m_type
    std::wstring ToWString(path_type type) const; //< Produces a string of type and does not cashe results
    explicit operator std::wstring() const; //< Cashes the string of m_type

    void set(const std::wstring& wstring, path_type type = default_path);
    path& operator=(const std::wstring& str);
#endif

  private:
    static std::vector<std::string> tokenize(const std::string& string, const std::string& delim);
    void generate_string_cashe() const;

  private:
    mutable bool m_dirty = true;
    mutable std::string m_string_cashe;

#if defined(_WIN32)
    mutable std::wstring m_wstring_cashe;
#endif

    path_type m_type;
    value_type m_path;
    bool m_absolute;
  };

  std::ostream& operator<<(std::ostream& os, const path& path);

  //Quick Functions
  path cwd();
  bool rm(const path& p);
  bool rm(const std::string& p);
  bool rmdir(const path& p);
  bool rmdir(const std::string& p);
  bool rmdirs(const path& p);           //<! Danger
  bool rmdirs(const std::string& p);    //<! Danger

  bool exists(const std::string& name);
  bool create_directory(const path& p);
  bool create_directories(const path& p);

  inline path absolute(const path p)
  {
    return p.make_absolute();
  }
  inline path normalize(const path p)
  {
    return p.make_normal();
  }
  inline bool is_directory(const path& p)
  {
    return p.is_directory();
  }
} // namespace filesystem
} //namespace biogears

#endif //BIOGEARS_COMMON_FILESYSTEM_PATH_H
