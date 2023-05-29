%{
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/properties/PropertyEnum.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>

%nspace BIOGEARS_NAMESPACE SESide;
%nspace BIOGEARS_NAMESPACE SEOpenClosed;
%nspace BIOGEARS_NAMESPACE SEOnOff;
%nspace BIOGEARS_NAMESPACE SEErrorType;

%rename(SESide_toString) BIOGEARS_NAMESPACE ToString(const SESide& e);
%rename(SEOpenClosed_toString) BIOGEARS_NAMESPACE ToString(const SEOpenClosed& e);
%rename(SEOnOff_toString) BIOGEARS_NAMESPACE ToString(const SEOnOff& e);
%rename(SEErrorType_toString) BIOGEARS_NAMESPACE ToString(const SEErrorType& e);

%ignore operator<<(std::ostream& os, const SESide& e);
%ignore operator<<(std::ostream& os, const SEOpenClosed& e);
%ignore operator<<(std::ostream& os, const SEOnOff& e);
%ignore operator<<(std::ostream& os, const SEErrorType& e);

%include <biogears/cdm/properties/PropertyEnum.h>

