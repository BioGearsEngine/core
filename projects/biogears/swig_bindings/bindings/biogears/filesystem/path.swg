

%{
   #include <stdexcept>
   #include <string>
   #include <vector>

  #include <biogears/common-exports.h>
  #include <biogears/filesystem/path.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%import <biogears/exports.swg>
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
%template (vector_filesystem_path)  std::vector<BIOGEARS_NAMESPACE filesystem::path>;
