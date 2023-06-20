%{
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
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
%import <biogears/cdm/substance/SESubstanceCompound.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>


%nspace BIOGEARS_NAMESPACE SESubstanceInfusion;

%ignore BIOGEARS_NAMESPACE SESubstanceInfusion::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceInfusion::Load;

%ignore BIOGEARS_NAMESPACE SESubstanceInfusion::ToString(std::ostream& str) const;

%include <biogears/cdm/patient/actions/SESubstanceInfusion.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SESubstanceInfusion*>;
%template (vector_SESubstanceInfusion_ptr)  std::vector<BIOGEARS_NAMESPACE SESubstanceInfusion*>;