%{
#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace BIOGEARS_NAMESPACE SEInfection;

%ignore BIOGEARS_NAMESPACE SEInfection::Unload;
%ignore BIOGEARS_NAMESPACE SEInfection::Load;

%ignore BIOGEARS_NAMESPACE SEInfection::SetLocation(const std::string& name);

%include <biogears/cdm/patient/actions/SEInfection.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEInfection*>;
%template (vector_SEInfection_ptr)  std::vector<BIOGEARS_NAMESPACE SEInfection*>;