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

%nspace BIOGEARS_NAMESPACE SEGastrointestinalSystem;
%nspace BIOGEARS_NAMESPACE SEDrugTransitState;

%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Load;

%rename (GetStomachContents_const) BIOGEARS_NAMESPACE SEGastrointestinalSystem::GetStomachContents() const;

OPEN_BIOGEARS_NAMESPACE
   class SEDrugTransitState;
CLOSE_BIOGEARS_NAMESPACE
%nspace std::map<const BIOGEARS_NAMESPACE SESubstance*, BIOGEARS_NAMESPACE SEDrugTransitState*>;
%template (map_SESubstance_ptr__SEDrugTransitState_ptr) std::map<const BIOGEARS_NAMESPACE SESubstance*, BIOGEARS_NAMESPACE SEDrugTransitState*>;

%include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>