

%{
#include <biogears/cdm/properties/SEProperty.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace BIOGEARS_NAMESPACE SEProperty;
%rename(Equals)  operator==(const SEProperty&) const;
%rename(NotEquals)  operator!=(const SEProperty&) const;

%ignore BIOGEARS_NAMESPACE SEProperty::Unload;
%ignore BIOGEARS_NAMESPACE SEProperty::Load;

%include <biogears/cdm/properties/SEProperty.h>


