%{
#include <biogears/cdm/substance/SESubstance.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/substance/SESubstanceAerosolization.swg>
%import <biogears/cdm/substance/SESubstanceClearance.swg>
%import <biogears/cdm/substance/SESubstancePharmacokinetics.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarAmountPerVolume.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarMassPerAmount.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTimePressure.swg>
%import <biogears/cdm/properties/SEScalarInversePressure.swg>
%import <biogears/cdm/properties/SEScalarMassPerAreaTime.swg>
%import <biogears/cdm/substance/SESubstancePharmacodynamics.swg>
%import <biogears/cdm/properties/SEScalarElectricResistance.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarTimeMassPerVolume.swg>

%nspace BIOGEARS_NAMESPACE SESubstance;

%ignore BIOGEARS_NAMESPACE SESubstance::Unload;
%ignore BIOGEARS_NAMESPACE SESubstance::Load;

%ignore BIOGEARS_NAMESPACE SESubstance::GetScalar(std::string const &);
%ignore BIOGEARS_NAMESPACE SESubstance::SetName(const std::string& name);

%rename (GetMichaelisCoefficient_const) BIOGEARS_NAMESPACE SESubstance::GetMichaelisCoefficient() const;
%rename (GetAerosolization_const) BIOGEARS_NAMESPACE SESubstance::GetAerosolization() const;
%rename (GetEndTidalFraction_const) BIOGEARS_NAMESPACE SESubstance::GetEndTidalFraction() const;
%rename (GetRelativeDiffusionCoefficient_const) BIOGEARS_NAMESPACE SESubstance::GetRelativeDiffusionCoefficient() const;
%rename (GetClearance_const) BIOGEARS_NAMESPACE SESubstance::GetClearance() const;
%rename (GetPK_const) BIOGEARS_NAMESPACE SESubstance::GetPK() const;
%rename (GetPD_const) BIOGEARS_NAMESPACE SESubstance::GetPD() const;

%include <biogears/cdm/substance/SESubstance.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SESubstance*>;
%template (vector_SESubstance_ptr)  std::vector<BIOGEARS_NAMESPACE SESubstance*>;

