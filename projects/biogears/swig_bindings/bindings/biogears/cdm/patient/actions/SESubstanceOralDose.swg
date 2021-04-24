%{
#include <biogears/cdm/patient/actions/SESubstanceOralDose.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SESubstanceAdministration.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace biogears::SESubstanceOralDose;
%nspace biogears::SETransmucosalState;

%ignore biogears::SESubstanceOralDose::Unload;
%ignore biogears::SESubstanceOralDose::Load;

namespace biogears {
  class SETransmucosalState;
}
%include <biogears/cdm/patient/actions/SESubstanceOralDose.h>