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
#include <string>
#include <sstream>
#include <iostream>

#include <biogears/exports.h>
  
namespace biogears {

  BIOGEARS_API std::string version_string();
  BIOGEARS_API char const *       version_string_str();
  BIOGEARS_API std::string full_version_string();
  BIOGEARS_API char const *       full_version_string_str();

  BIOGEARS_API std::string project_name();
  BIOGEARS_API char const *       project_name_str();

  BIOGEARS_API std::string rev_hash();
  BIOGEARS_API char const *       rev_hash_str();

  BIOGEARS_API std::string rev_tag();
  BIOGEARS_API char const *       rev_tag_str();

  BIOGEARS_API int rev_offset();

  BIOGEARS_API int biogears_major_version();
  BIOGEARS_API int biogears_minor_version();
  BIOGEARS_API int biogears_patch_version();

  BIOGEARS_API bool biogears_offical_release();

  BIOGEARS_API std::string rev_commit_date();
  BIOGEARS_API char const* rev_commit_date_str();
  BIOGEARS_API std::string biogears_build_date();
  BIOGEARS_API const char* biogears_build_date_str();\

  }
