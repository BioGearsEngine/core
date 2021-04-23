
// Add necessary symbols to generated header
%{
#include <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEAcuteRespiratoryDistress;

%ignore biogears::SEAcuteRespiratoryDistress::Unload;
%ignore biogears::SEAcuteRespiratoryDistress::Load;

%include <biogears/cdm/patient/actions/SEAcuteRespiratoryDistress.h>


