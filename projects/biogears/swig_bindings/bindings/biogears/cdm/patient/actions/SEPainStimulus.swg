%{
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
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

%nsspace  std::vector<BIOGEARS_NAMESPACE SEPainStimulus*>;
%template (vector_SEPainStimulus_ptr)  std::vector<BIOGEARS_NAMESPACE SEPainStimulus*>;