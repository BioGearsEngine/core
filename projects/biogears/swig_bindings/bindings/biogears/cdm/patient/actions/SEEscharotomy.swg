%{
#include <biogears/cdm/patient/actions/SEEscharotomy.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEEscharotomy;

%ignore biogears::SEEscharotomy::Unload;
%ignore biogears::SEEscharotomy::Load;

%ignore biogears::SEEscharotomy::SetLocation(std::string const&);

%include <biogears/cdm/patient/actions/SEEscharotomy.h>
