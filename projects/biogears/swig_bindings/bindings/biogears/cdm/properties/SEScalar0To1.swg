

%{
#include <biogears/cdm/properties/SEScalar0To1.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalar.swg>

%nspace biogears::SEScalar0To1;

%rename(Equals)  operator==(const SEScalar0To1&) const;
%rename(NotEquals)  operator!=(const SEScalar0To1&) const;

%include <biogears/cdm/properties/SEScalar0To1.h>


