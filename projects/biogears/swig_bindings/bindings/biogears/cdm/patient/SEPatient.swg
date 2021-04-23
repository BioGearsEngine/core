%{
#include <biogears/cdm/patient/SEPatient.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/utils/SEEventHandler.swg>
%import <biogears/cdm/patient/SENutrition.swg>
%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarLength.swg>
%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarFrequency.swg>
%import <biogears/cdm/properties/SEScalarNeg1To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>

%nspace biogears::SEPatient;

%ignore biogears::SEPatient::Unload;
%ignore biogears::SEPatient::Load;

%ignore biogears::SEPatient::GetScalar(std::string const &);
%ignore biogears::SEPatient::SetName(std::string const &);
%ignore biogears::SEPatient::SetAnnotation(std::string const &);

%rename(GetBodyFatFraction_const) biogears::SEPatient::GetBodyFatFraction() const;
%rename(GetHyperhidrosis_const) biogears::SEPatient::GetHyperhidrosis() const;
%rename(GetPainSusceptibility_const) biogears::SEPatient::GetPainSusceptibility() const;
%rename(GetRightLungRatio_const) biogears::SEPatient::GetRightLungRatio() const;


%include <biogears/cdm/patient/SEPatient.h>
