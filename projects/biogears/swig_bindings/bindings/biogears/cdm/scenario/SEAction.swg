%{
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace BIOGEARS_NAMESPACE SEAction;

%ignore BIOGEARS_NAMESPACE SEAction::Unload;
%ignore BIOGEARS_NAMESPACE SEAction::Load;
%ignore newFromBind(const CDM::ActionData& action, SESubstanceManager& substances);
%ignore SetComment(std::string const &);

%ignore SEAction::ToString(std::ostream& str) const;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEAction::ToString() const "public override";
#endif 

%include <biogears/cdm/scenario/SEAction.h>


