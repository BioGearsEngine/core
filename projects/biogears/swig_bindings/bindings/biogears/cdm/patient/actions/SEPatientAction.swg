%{
#include <biogears/cdm/patient/actions/SEPatientAction.h>
%}

%import <biogears/exports.swg>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/scenario/SEAction.swg>

%nspace biogears::SEPatientAction;

%ignore biogears::SEPatientAction::Unload;
%ignore biogears::SEPatientAction::Load;

#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEPatientAction::ToString() const "public override";
%csmethodmodifiers biogears::SEPatientAction::ToString(std::ostream& str) const "public override";
#endif 

%include <biogears/cdm/patient/actions/SEPatientAction.h>
