
%{
#include <biogears/cdm/scenario/requests/SETissueCompartmentDataRequest.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

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
