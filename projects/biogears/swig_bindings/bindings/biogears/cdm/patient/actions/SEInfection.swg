%{
#include <biogears/cdm/patient/actions/SEInfection.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace biogears::SEInfection;

%ignore biogears::SEInfection::Unload;
%ignore biogears::SEInfection::Load;

%ignore biogears::SEInfection::SetLocation(const std::string& name);

%include <biogears/cdm/patient/actions/SEInfection.h>
