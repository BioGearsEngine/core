// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarNeg1To1.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalar.swg>

%nspace biogears::SEScalarNeg1To1;

%rename(Equals)  operator==(const SEScalarNeg1To1&) const;
%rename(NotEquals)  operator!=(const SEScalarNeg1To1&) const;

%include <biogears/cdm/properties/SEScalarNeg1To1.h>