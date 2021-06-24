

%{
   #include <stdexcept>
   #include <string>
   #include <vector>

  #include <biogears/common-exports.h>
  #include <biogears/filesystem/path.h>
%}

%import <biogears/exports.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace biogears::filesystem::path;


%ignore biogears::filesystem::path::begin;
%ignore biogears::filesystem::path::end;
%ignore biogears::filesystem::path::path(std::string const&);
%ignore biogears::filesystem::path::path(path&&);
%ignore operator std::string() const; 

%rename(cd) operator/=(const biogears::filesystem::path& other);
%rename(concatinate) operator/(const  biogears::filesystem::path& other) const;
%rename(CopyFrom) operator=(const path& path);

%ignore operator=(path&& path);
%rename(Equals) operator==(const biogears::filesystem::path& p) const;
%rename(NotEquals) operator!=(const biogears::filesystem::path& p) const;

#if defined(SWIGCSHARP)
  %csmethodmodifiers   biogears::filesystem::path::ToString() const "public override";
  %ignore   biogears::filesystem::path::ToString(path_type type) const;
#endif

%include <biogears/filesystem/path.h>

%nsspace  std::vector<biogears::filesystem::path>;
%template (vector_filesystem_path)  std::vector<biogears::filesystem::path>;
