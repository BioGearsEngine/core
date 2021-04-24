%{
#include <biogears/cdm/scenario/SECondition.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace biogears::SECondition;

%ignore biogears::SECondition::Unload;
%ignore biogears::SECondition::Load;
%ignore newFromBind(const CDM::ConditionData& action, SESubstanceManager& substances);
%ignore SetComment(std::string const &);

%ignore SECondition::ToString(std::ostream& str) const;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SECondition::ToString() const "public override";
#endif 

%include <biogears/cdm/scenario/SECondition.h>


