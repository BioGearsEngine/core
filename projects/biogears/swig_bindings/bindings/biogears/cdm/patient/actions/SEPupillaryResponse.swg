%{
#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarNeg1To1.swg>

%nspace biogears::SEPupillaryResponse;

%ignore biogears::SEPupillaryResponse::Unload;
%ignore biogears::SEPupillaryResponse::Load;

%ignore biogears::SEPupillaryResponse::GetScalar(std::string const &);

%rename(GetReactivityModifier_const) biogears::SEPupillaryResponse::GetReactivityModifier() const;
%rename(GetShapeModifier_const) biogears::SEPupillaryResponse::GetShapeModifier() const;
%rename(GetSizeModifier_const) biogears::SEPupillaryResponse::GetSizeModifier() const;

%include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
