%{
#include <biogears/cdm/properties/SEHistogram.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEProperty.swg>

%nspace BIOGEARS_NAMESPACE SEHistogram;
%rename(Equals)  operator==(const SEHistogram&) const;
%rename(NotEquals)  operator!=(const SEHistogram&) const;

%ignore GetDependent() const;
%ignore GetIndependent() const;

%include <biogears/cdm/properties/SEHistogram.h>


