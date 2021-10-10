%{
#include <biogears/cdm/properties/SEFunction.h>
using namespace biogears;
using namespace CDM;
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEProperty.swg>

%nspace biogears::SEFunction;

%rename(Equals)  operator==(const SEFunction&) const;
%rename(NotEquals)  operator!=(const SEFunction&) const;

%include <biogears/cdm/properties/SEFunction.h>


