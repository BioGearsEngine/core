%{
#include <biogears/cdm/patient/actions/SESubstanceAdministration.h>
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
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SESubstanceAdministration;

%ignore BIOGEARS_NAMESPACE SESubstanceAdministration::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceAdministration::Load;
  
%include <biogears/cdm/patient/actions/SESubstanceAdministration.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SESubstanceAdministration*>;
%template (vector_SESubstanceAdministration_ptr)  std::vector<BIOGEARS_NAMESPACE SESubstanceAdministration*>;