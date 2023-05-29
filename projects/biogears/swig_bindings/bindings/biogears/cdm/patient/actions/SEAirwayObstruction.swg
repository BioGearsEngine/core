

%{
#include <biogears/cdm/patient/actions/SEAirwayObstruction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEAirwayObstruction;

%ignore BIOGEARS_NAMESPACE SEAirwayObstruction::Unload;
%ignore BIOGEARS_NAMESPACE SEAirwayObstruction::Load;

%include <biogears/cdm/patient/actions/SEAirwayObstruction.h>
