%{
#include <biogears/cdm/substance/SESubstanceFraction.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>

%nspace biogears::SESubstanceFraction;

%ignore biogears::SESubstanceFraction::Unload;
%ignore biogears::SESubstanceFraction::Load;

%rename (GetFractionAmount_const) biogears::SESubstanceFraction::GetFractionAmount() const;

%include <biogears/cdm/substance/SESubstanceFraction.h>

%nspace std::vector<biogears::SESubstanceFraction*>;
%nspace std::vector<const biogears::SESubstanceFraction*>;

%template(vector_SESubstanceFraction_ptr)  std::vector<biogears::SESubstanceFraction*>;
%template(vector_const_SSESubstanceFraction_ptr)  std::vector<const biogears::SESubstanceFraction*>;
