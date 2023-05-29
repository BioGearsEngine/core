%{
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SESubstanceAdministration.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceCompound.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>


%nspace BIOGEARS_NAMESPACE SESubstanceCompoundInfusion;

%ignore BIOGEARS_NAMESPACE SESubstanceCompoundInfusion::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceCompoundInfusion::Load;

%include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>