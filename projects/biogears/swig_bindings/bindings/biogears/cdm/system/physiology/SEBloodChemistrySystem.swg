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

%nspace biogears::SEBloodChemistrySystem;
%nspace biogears::SEInflammatoryResponse;

%ignore biogears::SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore biogears::SESubstancePharmacokinetics::Unload;
%ignore biogears::SESubstancePharmacokinetics::Load;

%rename (GetArterialBloodPH_const)  biogears::SEBloodChemistrySystem::GetArterialBloodPH() const ;
%rename (GetArterialBloodPHBaseline_const)  biogears::SEBloodChemistrySystem::GetArterialBloodPHBaseline() const ;
%rename (GetVenousBloodPH_const)  biogears::SEBloodChemistrySystem::GetVenousBloodPH() const ;
%rename (GetCarbonDioxideSaturation_const)  biogears::SEBloodChemistrySystem::GetCarbonDioxideSaturation() const ;
%rename (GetCarbonMonoxideSaturation_const)  biogears::SEBloodChemistrySystem::GetCarbonMonoxideSaturation() const;
%rename (GetHematocrit_const)  biogears::SEBloodChemistrySystem::GetHematocrit() const ;
%rename (GetOxygenSaturation_const)  biogears::SEBloodChemistrySystem::GetOxygenSaturation() const;
%rename (GetOxygenVenousSaturation_const)  biogears::SEBloodChemistrySystem::GetOxygenVenousSaturation() const ;
%rename (GetPulseOximetry_const)  biogears::SEBloodChemistrySystem::GetPulseOximetry() const;
%rename (GetShuntFraction_const)  biogears::SEBloodChemistrySystem::GetShuntFraction() const;
%rename (GetVolumeFractionNeutralPhospholipidInPlasma_const)  biogears::SEBloodChemistrySystem::GetVolumeFractionNeutralPhospholipidInPlasma() const ;
%rename (GetVolumeFractionNeutralLipidInPlasma_const)  biogears::SEBloodChemistrySystem::GetVolumeFractionNeutralLipidInPlasma() const;
%rename (GetInflammatoryResponse_const)  biogears::SEBloodChemistrySystem::GetInflammatoryResponse() const ;
%ignore  biogears::SEInflammatoryResponse::GetScalar(std::string const &);
%rename (GetLocalPathogen_const)  biogears::SEInflammatoryResponse::GetLocalPathogen() const ;
%rename (GetLocalBarrier_const)  biogears::SEInflammatoryResponse::GetLocalBarrier() const ;
%rename (GetLocalMacrophage_const)  biogears::SEInflammatoryResponse::GetLocalMacrophage() const ;
%rename (GetLocalNeutrophil_const)  biogears::SEInflammatoryResponse::GetLocalNeutrophil() const ;
%rename (GetAutonomicResponseLevel_const)  biogears::SEInflammatoryResponse::GetAutonomicResponseLevel() const ;
%rename (GetCatecholamines_const)  biogears::SEInflammatoryResponse::GetCatecholamines() const ;
%rename (GetConstitutiveNOS_const)  biogears::SEInflammatoryResponse::GetConstitutiveNOS() const;
%rename (GetInducibleNOSPre_const)  biogears::SEInflammatoryResponse::GetInducibleNOSPre() const ;
%rename (GetInducibleNOS_const)  biogears::SEInflammatoryResponse::GetInducibleNOS() const;
%rename (GetInterleukin6_const)  biogears::SEInflammatoryResponse::GetInterleukin6() const;
%rename (GetInterleukin10_const)  biogears::SEInflammatoryResponse::GetInterleukin10() const ;
%rename (GetInterleukin12_const)  biogears::SEInflammatoryResponse::GetInterleukin12() const ;
%rename (GetMacrophageResting_const)  biogears::SEInflammatoryResponse::GetMacrophageResting() const ;
%rename (GetMacrophageActive_const)  biogears::SEInflammatoryResponse::GetMacrophageActive() const ;
%rename (GetNeutrophilResting_const)  biogears::SEInflammatoryResponse::GetNeutrophilResting() const;
%rename (GetNeutrophilActive_const)  biogears::SEInflammatoryResponse::GetNeutrophilActive() const ;
%rename (GetNitrate_const)  biogears::SEInflammatoryResponse::GetNitrate() const ;
%rename (GetNitricOxide_const)  biogears::SEInflammatoryResponse::GetNitricOxide() const;
%rename (GetBloodPathogen_const)  biogears::SEInflammatoryResponse::GetBloodPathogen() const;
%rename (GetTissueIntegrity_const)  biogears::SEInflammatoryResponse::GetTissueIntegrity() const ;
%rename (GetTrauma_const)  biogears::SEInflammatoryResponse::GetTrauma() const ;
%rename (GetTumorNecrosisFactor_const)  biogears::SEInflammatoryResponse::GetTumorNecrosisFactor() const ;

namespace biogears{
  class SEInflammatoryResponse;
}

%include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>