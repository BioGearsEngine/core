%{
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace biogears::SEPainStimulus;

%ignore biogears::SEPainStimulus::Unload;
%ignore biogears::SEPainStimulus::Load;

%ignore biogears::SEPainStimulus::SetLocation(const std::string& name);

%include <biogears/cdm/patient/actions/SEPainStimulus.h>
