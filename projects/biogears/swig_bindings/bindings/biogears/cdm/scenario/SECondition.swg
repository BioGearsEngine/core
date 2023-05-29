%{
#include <biogears/cdm/scenario/SECondition.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace BIOGEARS_NAMESPACE SECondition;

%ignore BIOGEARS_NAMESPACE SECondition::Unload;
%ignore BIOGEARS_NAMESPACE SECondition::Load;
%ignore newFromBind(const CDM::ConditionData& action, SESubstanceManager& substances);
%ignore SetComment(std::string const &);

%ignore SECondition::ToString(std::ostream& str) const;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SECondition::ToString() const "public override";
#endif 

%include <biogears/cdm/scenario/SECondition.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SECondition*> ;
%template (vector_SECondition_ptr)  std::vector<BIOGEARS_NAMESPACE SECondition*> ;