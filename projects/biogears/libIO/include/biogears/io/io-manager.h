#pragma once

/**************************************************************************************
Copyright 2021 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/


#include <biogears/io-exports.h>
//!
//!  Functions for finding resource files that were part of the current biogears release
//!  These functions are encoded as part of libbiogears_io
//!
//!  Functions are exposed as either biogears::filesystem::path or char const * 
//!  TODO: Migrate Biogears C++17 and replace biogears::filesystem with std::filesystem

namespace biogears {
  namespace io {
    //!
    //!  IOManager is a class for interacting with the static functions in biogears::io
    //!  It simplfies interaction with the various resource_file_types at the cost of running
    //!  If Checks across the entire embeded library when testing for membership
    //!
    //!  TODO: Implmement behavior control
    class IOManager {
    private:
      bool _overwrite_mode = true;
      bool _error_if_not_empty = false;
      bool _transaction_based_writes = false;

      char* _staging_ground       = nullptr;
      char* _biogears_data_root   = nullptr;
      char* _biogears_schema_root = nullptr;
    public:
      BIOGEARS_IO_API bool generate_runtime_directory(const char* file);
      BIOGEARS_IO_API bool does_embedded_file_exist(const char* file);

      BIOGEARS_IO_API char const* find_resource_file(const char* file);
      BIOGEARS_IO_API char const* get_hash(const char* file);

      BIOGEARS_IO_API char const* get_embedded_resource_file(const char* file);
    };
  }
}