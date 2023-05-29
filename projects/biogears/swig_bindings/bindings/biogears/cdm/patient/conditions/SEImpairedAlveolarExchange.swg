%{
#include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace BIOGEARS_NAMESPACE SEImpairedAlveolarExchange;

%ignore BIOGEARS_NAMESPACE SEImpairedAlveolarExchange::Unload;
%ignore BIOGEARS_NAMESPACE SEImpairedAlveolarExchange::Load;

%ignore BIOGEARS_NAMESPACE SEImpairedAlveolarExchange::GetImpairedFraction() const;

%include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>
