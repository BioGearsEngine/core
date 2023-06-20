%{
#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%import <biogears/exports.swg>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/scenario/SEAction.swg>

%nspace BIOGEARS_NAMESPACE SEPatientAction;

%ignore BIOGEARS_NAMESPACE SEPatientAction::Unload;
%ignore BIOGEARS_NAMESPACE SEPatientAction::Load;

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEPatientAction::ToString() const "public override";
%csmethodmodifiers BIOGEARS_NAMESPACE SEPatientAction::ToString(std::ostream& str) const "public override";
#endif 

%include <biogears/cdm/patient/actions/SEPatientAction.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEPatientAction*>;
%template (vector_SEPatientAction_ptr)  std::vector<BIOGEARS_NAMESPACE SEPatientAction*>;