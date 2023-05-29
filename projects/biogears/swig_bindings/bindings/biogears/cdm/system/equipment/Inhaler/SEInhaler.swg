
// Add necessary symbols to generated header
%{
#include<biogears/cdm/system/equipment/Inhaler/SEInhaler.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>

%import <biogears/cdm/system/SESystem.swg>

%rename (GetNozzleLoss_const) BIOGEARS_NAMESPACE SEInhaler::GetNozzleLoss() const;

%include <biogears/cdm/system/equipment/Inhaler/SEInhaler.h>