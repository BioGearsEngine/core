// Add necessary symbols to generated header
%{
#include<biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>

%import <biogears/cdm/properties/SEScalarVolume.swg>

%ignore GetScalar(std::string const &);

%include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>