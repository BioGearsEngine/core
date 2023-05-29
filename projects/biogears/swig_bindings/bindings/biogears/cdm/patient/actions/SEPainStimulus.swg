%{
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace BIOGEARS_NAMESPACE SEPainStimulus;

%ignore BIOGEARS_NAMESPACE SEPainStimulus::Unload;
%ignore BIOGEARS_NAMESPACE SEPainStimulus::Load;

%ignore BIOGEARS_NAMESPACE SEPainStimulus::SetLocation(const std::string& name);

%include <biogears/cdm/patient/actions/SEPainStimulus.h>
