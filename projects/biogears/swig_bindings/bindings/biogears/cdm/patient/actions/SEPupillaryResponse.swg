%{
#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarNeg1To1.swg>

%nspace BIOGEARS_NAMESPACE SEPupillaryResponse;

%ignore BIOGEARS_NAMESPACE SEPupillaryResponse::Unload;
%ignore BIOGEARS_NAMESPACE SEPupillaryResponse::Load;

%ignore BIOGEARS_NAMESPACE SEPupillaryResponse::GetScalar(std::string const &);

%rename(GetReactivityModifier_const) BIOGEARS_NAMESPACE SEPupillaryResponse::GetReactivityModifier() const;
%rename(GetShapeModifier_const) BIOGEARS_NAMESPACE SEPupillaryResponse::GetShapeModifier() const;
%rename(GetSizeModifier_const) BIOGEARS_NAMESPACE SEPupillaryResponse::GetSizeModifier() const;

%include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
