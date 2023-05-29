%{
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarFrequency.swg>
%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>

%nspace BIOGEARS_NAMESPACE SESubstancePharmacodynamics;

%ignore BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePharmacodynamics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacodynamics::Load;

%rename (GetBronchodilation_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetBronchodilation() const;
%rename (GetDiastolicPressureModifier_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetDiastolicPressureModifier() const;
%rename (GetEMaxShapeParameter_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetEMaxShapeParameter() const;
%rename (GetFeverModifier_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetFeverModifier() const;
%rename (GetHeartRateModifier_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetHeartRateModifier() const;
%rename (GetHemorrhageModifier_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetHemorrhageModifier() const;
%rename (GetNeuromuscularBlock_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetNeuromuscularBlock() const;
%rename (GetPainModifier_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetPainModifier() const;
%rename (GetPupilReactivityModifier_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetPupilReactivityModifier() const;
%rename (GetPupilSizeModifier_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetPupilSizeModifier() const;
%rename (GetRespirationRateModifier_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetRespirationRateModifier() const;
%rename (GetSedation_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetSedation() const;
%rename (GetSystolicPressureModifier_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetSystolicPressureModifier() const;
%rename (GetTidalVolumeModifier_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetTidalVolumeModifier() const;
%rename (GetTubularPermeabilityModifier_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetTubularPermeabilityModifier() const;
%rename (GetCentralNervousModifier_const) BIOGEARS_NAMESPACE SESubstancePharmacodynamics::GetCentralNervousModifier() const;
%ignore BIOGEARS_NAMESPACE SEPharmacodynamicModifier::GetScalar(std::string const &);
%rename (GetEMax_const) BIOGEARS_NAMESPACE SEPharmacodynamicModifier::GetEMax() const;

OPEN_BIOGEARS_NAMESPACE
  class SEPharmacodynamicModifier;
CLOSE_BIOGEARS_NAMESPACE

%namespace std::map<std::string, BIOGEARS_NAMESPACE SEPharmacodynamicModifier*>;

%include <biogears/cdm/substance/SESubstancePharmacodynamics.h>

%template (map_string_SEPharmocodynamicModifier_ptr) std::map<std::string, BIOGEARS_NAMESPACE SEPharmacodynamicModifier*>;