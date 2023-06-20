%{
#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
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

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>


%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceFraction.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>


%nspace BIOGEARS_NAMESPACE SEMechanicalVentilation;

%ignore BIOGEARS_NAMESPACE SEMechanicalVentilation::Unload;
%ignore BIOGEARS_NAMESPACE SEMechanicalVentilation::Load;

%rename(GetFlow_const) BIOGEARS_NAMESPACE SEMechanicalVentilation::GetFlow(const VolumePerTimeUnit& unit) const;
%rename(GetPressure_const) BIOGEARS_NAMESPACE SEMechanicalVentilation::GetPressure(const PressureUnit& unit) const;

%rename(HasGasFractions) BIOGEARS_NAMESPACE SEMechanicalVentilation::HasGasFraction() const;
%rename(HasGasFraction_const) BIOGEARS_NAMESPACE SEMechanicalVentilation::HasGasFraction(const SESubstance& substance) const;
%rename(GetGasFractions_const) BIOGEARS_NAMESPACE SEMechanicalVentilation::GetGasFractions() const;
%rename(GetGasFraction_const) BIOGEARS_NAMESPACE SEMechanicalVentilation::GetGasFraction(const SESubstance& substance) const;  
  
%include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEMechanicalVentilation*>;
%template (vector_SEMechanicalVentilation_ptr)  std::vector<BIOGEARS_NAMESPACE SEMechanicalVentilation*>;