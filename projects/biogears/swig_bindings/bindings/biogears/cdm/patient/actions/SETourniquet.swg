%{
#include <biogears/cdm/patient/actions/SETourniquet.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SETourniquet;

%ignore BIOGEARS_NAMESPACE SETourniquet::Unload;
%ignore BIOGEARS_NAMESPACE SETourniquet::Load;

%ignore BIOGEARS_NAMESPACE SETourniquet::SetCompartment(const std::string& name);

%include <biogears/cdm/patient/actions/SETourniquet.h>
