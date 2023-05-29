%{
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/system/SESystem.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarAmountPerVolume.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>

%nspace BIOGEARS_NAMESPACE SEBloodChemistrySystem;
%nspace BIOGEARS_NAMESPACE SEInflammatoryResponse;

%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Load;

%rename (GetArterialBloodPH_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetArterialBloodPH() const ;
%rename (GetArterialBloodPHBaseline_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetArterialBloodPHBaseline() const ;
%rename (GetVenousBloodPH_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetVenousBloodPH() const ;
%rename (GetCarbonDioxideSaturation_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetCarbonDioxideSaturation() const ;
%rename (GetCarbonMonoxideSaturation_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetCarbonMonoxideSaturation() const;
%rename (GetHematocrit_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetHematocrit() const ;
%rename (GetOxygenSaturation_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetOxygenSaturation() const;
%rename (GetOxygenVenousSaturation_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetOxygenVenousSaturation() const ;
%rename (GetPulseOximetry_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetPulseOximetry() const;
%rename (GetShuntFraction_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetShuntFraction() const;
%rename (GetVolumeFractionNeutralPhospholipidInPlasma_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetVolumeFractionNeutralPhospholipidInPlasma() const ;
%rename (GetVolumeFractionNeutralLipidInPlasma_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetVolumeFractionNeutralLipidInPlasma() const;
%rename (GetInflammatoryResponse_const)  BIOGEARS_NAMESPACE SEBloodChemistrySystem::GetInflammatoryResponse() const ;
%ignore  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetScalar(std::string const &);
%rename (GetLocalPathogen_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetLocalPathogen() const ;
%rename (GetLocalBarrier_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetLocalBarrier() const ;
%rename (GetLocalMacrophage_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetLocalMacrophage() const ;
%rename (GetLocalNeutrophil_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetLocalNeutrophil() const ;
%rename (GetAutonomicResponseLevel_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetAutonomicResponseLevel() const ;
%rename (GetCatecholamines_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetCatecholamines() const ;
%rename (GetConstitutiveNOS_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetConstitutiveNOS() const;
%rename (GetInducibleNOSPre_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetInducibleNOSPre() const ;
%rename (GetInducibleNOS_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetInducibleNOS() const;
%rename (GetInterleukin6_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetInterleukin6() const;
%rename (GetInterleukin10_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetInterleukin10() const ;
%rename (GetInterleukin12_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetInterleukin12() const ;
%rename (GetMacrophageResting_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetMacrophageResting() const ;
%rename (GetMacrophageActive_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetMacrophageActive() const ;
%rename (GetNeutrophilResting_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetNeutrophilResting() const;
%rename (GetNeutrophilActive_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetNeutrophilActive() const ;
%rename (GetNitrate_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetNitrate() const ;
%rename (GetNitricOxide_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetNitricOxide() const;
%rename (GetBloodPathogen_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetBloodPathogen() const;
%rename (GetTissueIntegrity_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetTissueIntegrity() const ;
%rename (GetTrauma_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetTrauma() const ;
%rename (GetTumorNecrosisFactor_const)  BIOGEARS_NAMESPACE SEInflammatoryResponse::GetTumorNecrosisFactor() const ;

namespace biogears{
  class SEInflammatoryResponse;
}

%include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>