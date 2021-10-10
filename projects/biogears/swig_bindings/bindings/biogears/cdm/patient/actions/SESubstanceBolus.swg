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


%nspace biogears::SESubstanceBolus;

%ignore biogears::SESubstanceBolus::Unload;
%ignore biogears::SESubstanceBolus::Load;
%ignore biogears::SESubstanceBolus::ToString(std::ostream& str) const;

%include <biogears/cdm/patient/actions/SESubstanceBolus.h>