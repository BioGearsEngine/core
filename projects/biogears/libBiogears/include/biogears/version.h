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
#include <iostream>
#include <sstream>
#include <string>

#include <biogears/exports.h>

namespace biogears {

BIOGEARS_API std::string project_name();
constexpr BIOGEARS_API char const* project_name_str();

 BIOGEARS_API std::string version_string();
 BIOGEARS_API char const* version_string_str();
 BIOGEARS_API std::string branded_version_string();
 BIOGEARS_API char const* branded_version_string_str();
 BIOGEARS_API std::string full_version_string();
 BIOGEARS_API char const* full_version_string_str();
 BIOGEARS_API std::string branded_full_version_string();
 BIOGEARS_API char const* branded_full_version_string_str();

 BIOGEARS_API std::string rev_hash();
constexpr BIOGEARS_API char const* rev_hash_str();

 BIOGEARS_API std::string rev_tag();
constexpr BIOGEARS_API char const* rev_tag_str();

BIOGEARS_API int rev_offset();

constexpr BIOGEARS_API int biogears_major_version();
constexpr BIOGEARS_API int biogears_minor_version();
constexpr BIOGEARS_API int biogears_patch_version();

constexpr BIOGEARS_API bool biogears_offical_release();

 BIOGEARS_API std::string rev_commit_date();
constexpr BIOGEARS_API char const* rev_commit_date_str();
 BIOGEARS_API std::string biogears_build_date();
constexpr BIOGEARS_API const char* biogears_build_date_str();

}
