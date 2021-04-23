%{
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/properties/PropertyEnum.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>

%nspace biogears::SESide;
%nspace biogears::SEOpenClosed;
%nspace biogears::SEOnOff;
%nspace biogears::SEErrorType;

%rename(SESide_toString) biogears::ToString(const SESide& e);
%rename(SEOpenClosed_toString) biogears::ToString(const SEOpenClosed& e);
%rename(SEOnOff_toString) biogears::ToString(const SEOnOff& e);
%rename(SEErrorType_toString) biogears::ToString(const SEErrorType& e);

%ignore operator<<(std::ostream& os, const SESide& e);
%ignore operator<<(std::ostream& os, const SEOpenClosed& e);
%ignore operator<<(std::ostream& os, const SEOnOff& e);
%ignore operator<<(std::ostream& os, const SEErrorType& e);

%include <biogears/cdm/properties/PropertyEnum.h>

