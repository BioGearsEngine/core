
%{
   #include <stdexcept>
   #include <string>
   #include <vector>

  #include <biogears/filesystem/path.h>
  #include <biogears/io/io-manager.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%import <biogears/exports.swg>
%import <biogears/filesystem/path.swg>

%nspace BIOGEARS_NAMESPACE IOManager;

%ignore IOManager(IOManager&& iom);
%ignore BIOGEARS_NAMESPACE IOManager(BIOGEARS_NAMESPACE IOManager&& iom);
%ignore BIOGEARS_NAMESPACE IOManager::IOManager(BIOGEARS_NAMESPACE IOManager&& iom);
%ignore BIOGEARS_NAMESPACE IOManager::IOManager(IOManager&& iom);

%ignore  BIOGEARS_NAMESPACE IOManager::operator=(IOManager&& rhs);
%ignore  BIOGEARS_NAMESPACE IOManager::operator=(IOManager const& rhs);

%include <biogears/io/io-manager.h>
