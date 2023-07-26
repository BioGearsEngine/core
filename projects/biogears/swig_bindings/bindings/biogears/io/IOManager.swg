
%{
   #include <stdexcept>
   #include <string>
   #include <vector>

  #include <biogears/filesystem/path.h>
  #include <biogears/io/io-manager.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) IOManager
#endif

%include <biogears/exports.swg>
%import <biogears/filesystem/path.swg>

%nspace BIOGEARS_NAMESPACE IOManager;

%ignore IOManager(IOManager&& iom);
%ignore BIOGEARS_NAMESPACE IOManager(BIOGEARS_NAMESPACE IOManager&& iom);
%ignore BIOGEARS_NAMESPACE IOManager::IOManager(BIOGEARS_NAMESPACE IOManager&& iom);
%ignore BIOGEARS_NAMESPACE IOManager::IOManager(IOManager&& iom);

%ignore  BIOGEARS_NAMESPACE IOManager::operator=(IOManager&& rhs);
%ignore  BIOGEARS_NAMESPACE IOManager::operator=(IOManager const& rhs);

%include <biogears/io/io-manager.h>
