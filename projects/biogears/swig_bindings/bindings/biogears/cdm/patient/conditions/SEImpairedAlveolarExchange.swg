%{
#include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace biogears::SEImpairedAlveolarExchange;

%ignore biogears::SEImpairedAlveolarExchange::Unload;
%ignore biogears::SEImpairedAlveolarExchange::Load;

%ignore biogears::SEImpairedAlveolarExchange::GetImpairedFraction() const;

%include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>
