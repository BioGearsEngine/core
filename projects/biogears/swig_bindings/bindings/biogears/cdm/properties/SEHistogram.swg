%{
#include <biogears/cdm/properties/SEHistogram.h>
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
) SEHistogram
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEProperty.swg>

%nspace BIOGEARS_NAMESPACE SEHistogram;
%rename(Equals)  operator==(const SEHistogram&) const;
%rename(NotEquals)  operator!=(const SEHistogram&) const;

%ignore GetDependent() const;
%ignore GetIndependent() const;

%include <biogears/cdm/properties/SEHistogram.h>


