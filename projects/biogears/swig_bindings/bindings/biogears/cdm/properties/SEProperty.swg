
// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEProperty.h>
%}

%include <biogears/exports.h>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace biogears::SEProperty;
%rename(Equals)  operator==(const SEProperty&) const;
%rename(NotEquals)  operator!=(const SEProperty&) const;

%ignore biogears::SEProperty::Unload;
%ignore biogears::SEProperty::Load;

%include <biogears/cdm/properties/SEProperty.h>


