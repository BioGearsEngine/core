
%{
#include <biogears/cdm/properties/SEScalarNeg1To1.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalar.swg>

%nspace BIOGEARS_NAMESPACE SEScalarNeg1To1;

%rename(Equals)  operator==(const SEScalarNeg1To1&) const;
%rename(NotEquals)  operator!=(const SEScalarNeg1To1&) const;

%include <biogears/cdm/properties/SEScalarNeg1To1.h>