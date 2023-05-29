%{
#include <biogears/cdm/properties/SEFunctionElectricPotentialVsTime.h>
%}

%include <biogears/exports.h>

%rename(Equals)  operator==(const SEFunctionElectricPotentialVsTime&) const;
%rename(NotEquals)  operator!=(const SEFunctionElectricPotentialVsTime&) const;

%nspace std::vector<double>;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<TimeUnit>;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<ElectricPotentialUnit>;

%import <biogears/cdm/properties/SEFunction.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/properties/SEScalarElectricPotential.swg>

%include <biogears/cdm/properties/SEFunctionElectricPotentialVsTime.h>


