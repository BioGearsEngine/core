%{
#include <biogears/cdm/properties/SEScalar0To1.h>
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
) SEScalar0To1
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%nspace BIOGEARS_NAMESPACE SEScalar0To1;

%rename(Equals)  operator==(const SEScalar0To1&) const;
%rename(NotEquals)  operator!=(const SEScalar0To1&) const;

%include <biogears/cdm/properties/SEScalar0To1.h>


