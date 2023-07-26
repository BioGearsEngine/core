%{
#include <biogears/cdm/scenario/requests/SECompartmentDataRequest.h>
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
) SECompartmentDataRequest
#endif

%include <biogears/exports.swg>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <std_map.i>

%import <biogears/cdm/scenario/requests/SEDataRequest.swg>

%nspace BIOGEARS_NAMESPACE SECompartmentDataRequest;

%ignore SetCompartment(const std::string& name);
%ignore Set(const std::string& cmpt, const std::string& name, const std::string& unit = "");
%ignore Set(const char* cmpt, const char* name, const CCompoundUnit& unit);
%ignore Set(const std::string& cmpt, const std::string& name, const CCompoundUnit& unit);

#if defined(SWIGCSHARP)
//%csmethodmodifiers BIOGEARS_NAMESPACE SECompartmentDataRequest::Set(const char* cmpt, const char* name, const char* unit = "") "public";
#endif  

%include <biogears/cdm/scenario/requests/SECompartmentDataRequest.h>
