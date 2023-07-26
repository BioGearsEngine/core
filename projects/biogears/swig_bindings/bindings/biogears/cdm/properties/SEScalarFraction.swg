%{
#include <biogears/cdm/properties/SEScalarFraction.h>
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
) SEScalarFraction
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%nspace BIOGEARS_NAMESPACE SEScalarFraction;

%rename(Equals)  operator==(const SEScalarFraction&) const;
%rename(NotEquals)  operator!=(const SEScalarFraction&) const;

%include <biogears/cdm/properties/SEScalarFraction.h>