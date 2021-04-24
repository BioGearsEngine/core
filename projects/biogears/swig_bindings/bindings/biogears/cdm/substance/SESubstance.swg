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

%nspace biogears::SESubstance;

%ignore biogears::SESubstance::Unload;
%ignore biogears::SESubstance::Load;

%ignore biogears::SESubstance::GetScalar(std::string const &);
%ignore biogears::SESubstance::SetName(const std::string& name);

%rename (GetMichaelisCoefficient_const) biogears::SESubstance::GetMichaelisCoefficient() const;
%rename (GetAerosolization_const) biogears::SESubstance::GetAerosolization() const;
%rename (GetEndTidalFraction_const) biogears::SESubstance::GetEndTidalFraction() const;
%rename (GetRelativeDiffusionCoefficient_const) biogears::SESubstance::GetRelativeDiffusionCoefficient() const;
%rename (GetClearance_const) biogears::SESubstance::GetClearance() const;
%rename (GetPK_const) biogears::SESubstance::GetPK() const;
%rename (GetPD_const) biogears::SESubstance::GetPD() const;

%include <biogears/cdm/substance/SESubstance.h>

%nsspace  std::vector<biogears::SESubstance*>;
%template (vector_SESubstance_ptr)  std::vector<biogears::SESubstance*>;

