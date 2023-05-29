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

%nspace BIOGEARS_NAMESPACE SESubstancePharmacokinetics;

%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Load;

%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::HasTissueKinetics(const std::string& name) const;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetTissueKinetics(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetTissueKinetics(const std::string& name) const;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::RemoveTissueKinetics(const std::string& name);

%rename (GetPhysicochemicals_const)BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetPhysicochemicals() const;
%rename (GetTissueKinetics_const)BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetTissueKinetics(char const *) const;

OPEN_BIOGEARS_NAMESPACE
  class SEPharmacokineticsModifier;
CLOSE_BIOGEARS_NAMESPACE

%include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
