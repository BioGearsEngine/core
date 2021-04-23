
// Add necessary symbols to generated header
%{
#include <biogears/cdm/scenario/SEAction.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace biogears::SEProperty;

%ignore biogears::SEAction::Unload;
%ignore biogears::SEAction::Load;
%ignore newFromBind(const CDM::ActionData& action, SESubstanceManager& substances);
%ignore SetComment(std::string const &);
%include <biogears/cdm/scenario/SEAction.h>


