%{
#include <biogears/cdm/patient/actions/SENeedleDecompression.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SENeedleDecompression;

%ignore biogears::SENeedleDecompression::Unload;
%ignore biogears::SENeedleDecompression::Load;

%include <biogears/cdm/patient/actions/SENeedleDecompression.h>
