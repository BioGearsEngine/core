
%{
#include <biogears/cdm/scenario/requests/SETissueCompartmentDataRequest.h>
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
) SETissueCompartmentDataRequest
#endif

%include <biogears/exports.swg>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <std_map.i>

%import <biogears/cdm/scenario/requests/SECompartmentDataRequest.swg>

%nspace BIOGEARS_NAMESPACE SETissueCompartmentDataRequest;


#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SETissueCompartmentDataRequest::HashCode() const "public override";
#endif 

%include <biogears/cdm/scenario/requests/SETissueCompartmentDataRequest.h>
