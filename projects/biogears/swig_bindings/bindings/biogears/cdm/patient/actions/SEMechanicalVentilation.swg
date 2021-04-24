%{
#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>


%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceFraction.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>


%nspace biogears::SEMechanicalVentilation;

%ignore biogears::SEMechanicalVentilation::Unload;
%ignore biogears::SEMechanicalVentilation::Load;

%rename(GetFlow_const) biogears::SEMechanicalVentilation::GetFlow(const VolumePerTimeUnit& unit) const;
%rename(GetPressure_const) biogears::SEMechanicalVentilation::GetPressure(const PressureUnit& unit) const;

%rename(HasGasFractions) biogears::SEMechanicalVentilation::HasGasFraction() const;
%rename(HasGasFraction_const) biogears::SEMechanicalVentilation::HasGasFraction(const SESubstance& substance) const;
%rename(GetGasFractions_const) biogears::SEMechanicalVentilation::GetGasFractions() const;
%rename(GetGasFraction_const) biogears::SEMechanicalVentilation::GetGasFraction(const SESubstance& substance) const;  
  
%include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
