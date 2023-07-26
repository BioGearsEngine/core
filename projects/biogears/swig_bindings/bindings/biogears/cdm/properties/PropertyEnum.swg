%{
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/properties/PropertyEnum.h>
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
) PropertyEnum
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/CommonDataModel.swg>

%nspace BIOGEARS_NAMESPACE SESide;
%nspace BIOGEARS_NAMESPACE SEOpenClosed;
%nspace BIOGEARS_NAMESPACE SEOnOff;
%nspace BIOGEARS_NAMESPACE SEErrorType;

%rename(SESide_toString) BIOGEARS_NAMESPACE ToString(const SESide& e);
%rename(SEOpenClosed_toString) BIOGEARS_NAMESPACE ToString(const SEOpenClosed& e);
%rename(SEOnOff_toString) BIOGEARS_NAMESPACE ToString(const SEOnOff& e);
%rename(SEErrorType_toString) BIOGEARS_NAMESPACE ToString(const SEErrorType& e);

%ignore operator<<(std::ostream& os, const SESide& e);
%ignore operator<<(std::ostream& os, const SEOpenClosed& e);
%ignore operator<<(std::ostream& os, const SEOnOff& e);
%ignore operator<<(std::ostream& os, const SEErrorType& e);

%include <biogears/cdm/properties/PropertyEnum.h>

