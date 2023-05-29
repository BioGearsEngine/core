%{
#include <biogears/cdm/patient/actions/SESubstanceOralDose.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SESubstanceAdministration.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace BIOGEARS_NAMESPACE SESubstanceOralDose;
%nspace BIOGEARS_NAMESPACE SETransmucosalState;

%ignore BIOGEARS_NAMESPACE SESubstanceOralDose::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceOralDose::Load;

OPEN_BIOGEARS_NAMESPACE
  class SETransmucosalState;
CLOSE_BIOGEARS_NAMESPACE
%include <biogears/cdm/patient/actions/SESubstanceOralDose.h>
