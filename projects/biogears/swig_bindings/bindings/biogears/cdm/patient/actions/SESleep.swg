%{
#include <biogears/cdm/patient/actions/SESleep.h>
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
) SESleep
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SESleep;

%ignore BIOGEARS_NAMESPACE SESleep::Unload;
%ignore BIOGEARS_NAMESPACE SESleep::Load;

%include <biogears/cdm/patient/actions/SESleep.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SESleep*>;
%template (vector_SESleep_ptr)  std::vector<BIOGEARS_NAMESPACE SESleep*>;
