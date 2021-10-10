%{
#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
using namespace biogears;
using namespace CDM;
%}

%include <biogears/exports.h>

%rename(Equals)  operator==(const SEFunctionElectricPotentialVsTime&) const;
%rename(NotEquals)  operator!=(const SEFunctionElectricPotentialVsTime&) const;

%nspace std::vector<double>;
%nspace biogears::SEScalarQuantity<TimeUnit>;
%nspace biogears::SEScalarQuantity<ElectricPotentialUnit>;

%import <biogears/cdm/properties/SEFunction.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>

%include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>


