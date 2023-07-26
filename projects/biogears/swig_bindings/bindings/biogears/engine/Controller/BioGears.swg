%{
  #include <biogears/container/Tree.tci.h>
  #include <biogears/cdm/utils/Logger.h>
  #include <biogears/engine/Controller/BioGears.h>
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
) BioGears
#endif

%include <biogears/exports.swg>
%import <biogears/cdm/utils/Logger.swg>

%nspace BIOGEARS_NAMESPACE BioGears;

OPEN_BIOGEARS_NAMESPACE
  class BioGears : public Loggable{
    BioGears (Logger* logger);
  };
CLOSE_BIOGEARS_NAMESPACE
