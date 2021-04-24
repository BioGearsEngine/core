%{
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/system/SESystem.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTimeArea.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarFrequency.swg>
%import <biogears/cdm/properties/SEScalarFlowResistance.swg>
%import <biogears/cdm/properties/SEScalarPressureTimePerVolumeArea.swg>

%nspace biogears::SECardiovascularSystem;

%ignore biogears::SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore biogears::SESubstancePharmacokinetics::Unload;
%ignore biogears::SESubstancePharmacokinetics::Load;

%rename (GetHeartEjectionFraction_const) biogears::SECardiovascularSystem::GetHeartEjectionFraction() const;

%include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
