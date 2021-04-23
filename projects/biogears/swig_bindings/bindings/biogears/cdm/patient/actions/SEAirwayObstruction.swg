

%{
#include <biogears/cdm/patient/actions/SEAirwayObstruction.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEAirwayObstruction;

%ignore biogears::SEAirwayObstruction::Unload;
%ignore biogears::SEAirwayObstruction::Load;

%include <biogears/cdm/patient/actions/SEAirwayObstruction.h>
