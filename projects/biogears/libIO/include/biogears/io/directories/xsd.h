#pragma once

#include <biogears/io-exports.h>
#include <biogears/filesystem/path.h>

//!
//!  Functions for finding resource files that were part of the current biogears release
//!  These functions are encoded as part of libbiogears_io
//!
//!  Functions are exposed as either biogears::filesystem::path or char const * 
//!  TODO: Migrate Biogears C++17 and replace biogears::filesystem with std::filesystem

namespace biogears {
  namespace io {

    BIOGEARS_IO_API bool  generate_xsd_directory(const char* file);
    BIOGEARS_IO_API bool does_embedded_xsd_file_exist(const char* file);
    
    BIOGEARS_IO_API  char const *  find_xsd_file(const char* file, const char* root = nullptr);
    BIOGEARS_IO_API  char const *  get_xsd_file_hash (const char* file);
 
    BIOGEARS_IO_API char const * const *  list_xsd_files();
    BIOGEARS_IO_API size_t  xsd_file_count();

    BIOGEARS_IO_API char const *  get_embedded_xsd_file( const char* file);

        //biogears::filesystem::path overloads
    BIOGEARS_IO_API bool  generate_xsd_directory(biogears::filesystem::path file);
    BIOGEARS_IO_API bool does_embedded_xsd_file_exist(biogears::filesystem::path file);
    
    BIOGEARS_IO_API char const *  get_xsd_file_hash (biogears::filesystem::path file);
    BIOGEARS_IO_API char const *  get_embedded_xsd_file( biogears::filesystem::path file);
  }
}