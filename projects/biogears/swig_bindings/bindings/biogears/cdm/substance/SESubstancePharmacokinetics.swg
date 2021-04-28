%{
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/substance/SESubstancePhysicochemicals.swg>
%import <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.swg>

%nspace biogears::SESubstancePharmacokinetics;

%ignore biogears::SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore biogears::SESubstancePharmacokinetics::Unload;
%ignore biogears::SESubstancePharmacokinetics::Load;

%ignore biogears::SESubstancePharmacokinetics::HasTissueKinetics(const std::string& name) const;
%ignore biogears::SESubstancePharmacokinetics::GetTissueKinetics(const std::string& name);
%ignore biogears::SESubstancePharmacokinetics::GetTissueKinetics(const std::string& name) const;
%ignore biogears::SESubstancePharmacokinetics::RemoveTissueKinetics(const std::string& name);

%rename (GetPhysicochemicals_const)biogears::SESubstancePharmacokinetics::GetPhysicochemicals() const;
%rename (GetTissueKinetics_const)biogears::SESubstancePharmacokinetics::GetTissueKinetics(char const *) const;

namespace biogears {
  class SEPharmacokineticsModifier;
}

%include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
