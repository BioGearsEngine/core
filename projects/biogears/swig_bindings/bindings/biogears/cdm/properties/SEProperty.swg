%{
#include <biogears/cdm/properties/SEProperty.h>
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
) SEProperty
#endif

%include <biogears/exports.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace BIOGEARS_NAMESPACE SEProperty;
%rename(Equals)  operator==(const SEProperty&) const;
%rename(NotEquals)  operator!=(const SEProperty&) const;

%ignore BIOGEARS_NAMESPACE SEProperty::Unload;
%ignore BIOGEARS_NAMESPACE SEProperty::Load;

%include <biogears/cdm/properties/SEProperty.h>


