%{
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarFrequency.swg>
%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>

%nspace biogears::SESubstancePharmacodynamics;

%ignore biogears::SESubstancePharmacodynamics::GetScalar(const std::string& name);
%ignore biogears::SESubstancePharmacodynamics::Unload;
%ignore biogears::SESubstancePharmacodynamics::Load;

%rename (GetBronchodilation_const) biogears::SESubstancePharmacodynamics::GetBronchodilation() const;
%rename (GetDiastolicPressureModifier_const) biogears::SESubstancePharmacodynamics::GetDiastolicPressureModifier() const;
%rename (GetEMaxShapeParameter_const) biogears::SESubstancePharmacodynamics::GetEMaxShapeParameter() const;
%rename (GetFeverModifier_const) biogears::SESubstancePharmacodynamics::GetFeverModifier() const;
%rename (GetHeartRateModifier_const) biogears::SESubstancePharmacodynamics::GetHeartRateModifier() const;
%rename (GetHemorrhageModifier_const) biogears::SESubstancePharmacodynamics::GetHemorrhageModifier() const;
%rename (GetNeuromuscularBlock_const) biogears::SESubstancePharmacodynamics::GetNeuromuscularBlock() const;
%rename (GetPainModifier_const) biogears::SESubstancePharmacodynamics::GetPainModifier() const;
%rename (GetPupilReactivityModifier_const) biogears::SESubstancePharmacodynamics::GetPupilReactivityModifier() const;
%rename (GetPupilSizeModifier_const) biogears::SESubstancePharmacodynamics::GetPupilSizeModifier() const;
%rename (GetRespirationRateModifier_const) biogears::SESubstancePharmacodynamics::GetRespirationRateModifier() const;
%rename (GetSedation_const) biogears::SESubstancePharmacodynamics::GetSedation() const;
%rename (GetSystolicPressureModifier_const) biogears::SESubstancePharmacodynamics::GetSystolicPressureModifier() const;
%rename (GetTidalVolumeModifier_const) biogears::SESubstancePharmacodynamics::GetTidalVolumeModifier() const;
%rename (GetTubularPermeabilityModifier_const) biogears::SESubstancePharmacodynamics::GetTubularPermeabilityModifier() const;
%rename (GetCentralNervousModifier_const) biogears::SESubstancePharmacodynamics::GetCentralNervousModifier() const;
%ignore biogears::SEPharmacodynamicModifier::GetScalar(std::string const &);
%rename (GetEMax_const) biogears::SEPharmacodynamicModifier::GetEMax() const;

namespace biogears {
  class SEPharmacodynamicModifier;
}

%namespace std::map<std::string, biogears::SEPharmacodynamicModifier*>;

%include <biogears/cdm/substance/SESubstancePharmacodynamics.h>

%template (map_string_SEPharmocodynamicModifier_ptr) std::map<std::string, biogears::SEPharmacodynamicModifier*>;