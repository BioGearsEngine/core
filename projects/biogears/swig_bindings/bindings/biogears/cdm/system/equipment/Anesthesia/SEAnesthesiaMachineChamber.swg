// Add necessary symbols to generated header
%{
#include<biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>

%rename(GetSubstanceFraction_const)  GetSubstanceFraction() const;

%ignore GetScalar(const std::string& name) override;


%include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>