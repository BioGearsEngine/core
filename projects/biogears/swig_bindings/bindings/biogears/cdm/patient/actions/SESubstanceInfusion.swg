%{
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SESubstanceAdministration.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceCompound.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>


%nspace biogears::SESubstanceInfusion;

%ignore biogears::SESubstanceInfusion::Unload;
%ignore biogears::SESubstanceInfusion::Load;

%ignore biogears::SESubstanceInfusion::ToString(std::ostream& str) const;

%include <biogears/cdm/patient/actions/SESubstanceInfusion.h>