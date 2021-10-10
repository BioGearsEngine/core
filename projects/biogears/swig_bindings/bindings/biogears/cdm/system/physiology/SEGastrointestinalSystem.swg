%{
#include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/system/SESystem.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/patient/SENutrition.swg>

%nspace biogears::SEGastrointestinalSystem;
%nspace biogears::SEDrugTransitState;

%ignore biogears::SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore biogears::SESubstancePharmacokinetics::Unload;
%ignore biogears::SESubstancePharmacokinetics::Load;

%rename (GetStomachContents_const) biogears::SEGastrointestinalSystem::GetStomachContents() const;

namespace biogears {
   class SEDrugTransitState;
}
%nspace std::map<const biogears::SESubstance*, biogears::SEDrugTransitState*>;
%template (map_SESubstance_ptr__SEDrugTransitState_ptr) std::map<const biogears::SESubstance*, biogears::SEDrugTransitState*>;

%include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>