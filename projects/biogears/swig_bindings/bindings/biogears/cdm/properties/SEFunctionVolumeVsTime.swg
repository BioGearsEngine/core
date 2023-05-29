%{
#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
USING_BIOGEARS_NAMESPACE;
using namespace CDM;
%}

%include <biogears/exports.h>
%include <biogears/config.h>

%rename(Equals)  operator==(const SEFunctionElectricPotentialVsTime&) const;
%rename(NotEquals)  operator!=(const SEFunctionElectricPotentialVsTime&) const;

%nspace std::vector<double>;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<TimeUnit>;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<ElectricPotentialUnit>;

%import <biogears/cdm/properties/SEFunction.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>

%include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>


