%{
#include <biogears/cdm/properties/SEFunction.h>
USING_BIOGEARS_NAMESPACE;
using namespace CDM;
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEProperty.swg>

%nspace BIOGEARS_NAMESPACE SEFunction;

%rename(Equals)  operator==(const SEFunction&) const;
%rename(NotEquals)  operator!=(const SEFunction&) const;

%include <biogears/cdm/properties/SEFunction.h>


