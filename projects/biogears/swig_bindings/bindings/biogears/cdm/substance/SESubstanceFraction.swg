%{
#include <biogears/cdm/substance/SESubstanceFraction.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>

%nspace BIOGEARS_NAMESPACE SESubstanceFraction;

%ignore BIOGEARS_NAMESPACE SESubstanceFraction::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceFraction::Load;

%rename (GetFractionAmount_const) BIOGEARS_NAMESPACE SESubstanceFraction::GetFractionAmount() const;

%include <biogears/cdm/substance/SESubstanceFraction.h>

%nspace std::vector<BIOGEARS_NAMESPACE SESubstanceFraction*>;
%nspace std::vector<const BIOGEARS_NAMESPACE SESubstanceFraction*>;

%template(vector_SESubstanceFraction_ptr)  std::vector<BIOGEARS_NAMESPACE SESubstanceFraction*>;
%template(vector_const_SSESubstanceFraction_ptr)  std::vector<const BIOGEARS_NAMESPACE SESubstanceFraction*>;
