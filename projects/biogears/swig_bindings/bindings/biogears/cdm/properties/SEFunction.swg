%{
#include <biogears/cdm/properties/SEFunction.h>
USING_BIOGEARS_NAMESPACE;
using namespace CDM;
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
) SEFunction
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEProperty.swg>

%nspace BIOGEARS_NAMESPACE SEFunction;

%rename(Equals)  operator==(const SEFunction&) const;
%rename(NotEquals)  operator!=(const SEFunction&) const;

%include <biogears/cdm/properties/SEFunction.h>


