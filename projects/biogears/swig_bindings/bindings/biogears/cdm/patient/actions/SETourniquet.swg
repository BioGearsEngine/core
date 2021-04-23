%{
#include <biogears/cdm/patient/actions/SETourniquet.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SETourniquet;

%ignore biogears::SETourniquet::Unload;
%ignore biogears::SETourniquet::Load;

%ignore biogears::SETourniquet::SetCompartment(const std::string& name);

%include <biogears/cdm/patient/actions/SETourniquet.h>
