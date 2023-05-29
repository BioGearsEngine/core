%{
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalar.swg>

%nspace BIOGEARS_NAMESPACE SEScalarFraction;

%rename(Equals)  operator==(const SEScalarFraction&) const;
%rename(NotEquals)  operator!=(const SEScalarFraction&) const;

%include <biogears/cdm/properties/SEScalarFraction.h>