%{
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SESubstanceAdministration.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceCompound.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>


%nspace BIOGEARS_NAMESPACE SESubstanceBolus;

%ignore BIOGEARS_NAMESPACE SESubstanceBolus::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceBolus::Load;
%ignore BIOGEARS_NAMESPACE SESubstanceBolus::ToString(std::ostream& str) const;

%include <biogears/cdm/patient/actions/SESubstanceBolus.h>