%{
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalarFlowResistance.swg>

%nspace biogears::SEHemorrhage;

%ignore biogears::SEHemorrhage::Unload;
%ignore biogears::SEHemorrhage::Load;

%ignore biogears::SEHemorrhage::SetCompartment(std::string const &);


%rename(GetMCIS_Const) biogears::SEHemorrhage::GetMCIS() const;
%rename(GetInitialRate_Const) biogears::SEHemorrhage::GetInitialRate() const;
%rename(GetBleedResistance_Const) biogears::SEHemorrhage::GetBleedResistance() const;

%include <biogears/cdm/patient/actions/SEHemorrhage.h>
