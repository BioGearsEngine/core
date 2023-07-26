#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) path
#endif
%{
   #include <stdexcept>
   #include <string>
   #include <vector>

  #include <biogears/common-exports.h>
  #include <biogears/filesystem/path.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace BIOGEARS_NAMESPACE filesystem::path;


%ignore BIOGEARS_NAMESPACE filesystem::path::begin;
%ignore BIOGEARS_NAMESPACE filesystem::path::end;
%ignore BIOGEARS_NAMESPACE filesystem::path::path(std::string const&);
%ignore BIOGEARS_NAMESPACE filesystem::path::path(path&&);
%ignore operator std::string() const; 

%rename(cd) operator/=(const BIOGEARS_NAMESPACE filesystem::path& other);
%rename(concatinate) operator/(const  BIOGEARS_NAMESPACE filesystem::path& other) const;
%rename(CopyFrom) operator=(const path& path);

%ignore operator=(path&& path);
%rename(Equals) operator==(const BIOGEARS_NAMESPACE filesystem::path& p) const;
%rename(NotEquals) operator!=(const BIOGEARS_NAMESPACE filesystem::path& p) const;

#if defined(SWIGCSHARP)
  %csmethodmodifiers   BIOGEARS_NAMESPACE filesystem::path::ToString() const "public override";
  %ignore   BIOGEARS_NAMESPACE filesystem::path::ToString(path_type type) const;
#endif

%include <biogears/filesystem/path.h>



%nsspace  std::vector<BIOGEARS_NAMESPACE filesystem::path>;
%traits_swigtype(::path);
%fragment(SWIG_Traits_frag(filesystem::path));
%template (vector_filesystem_path)  std::vector<BIOGEARS_NAMESPACE filesystem::path>;
