%{
#include <biogears/cdm/scenario/SEAction.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace biogears::SEAction;

%ignore biogears::SEAction::Unload;
%ignore biogears::SEAction::Load;
%ignore newFromBind(const CDM::ActionData& action, SESubstanceManager& substances);
%ignore SetComment(std::string const &);

%ignore SEAction::ToString(std::ostream& str) const;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEAction::ToString() const "public override";
#endif 

%include <biogears/cdm/scenario/SEAction.h>


