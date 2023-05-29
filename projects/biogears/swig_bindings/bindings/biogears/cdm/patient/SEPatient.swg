%{
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%import <biogears/exports.swg>

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

%nspace BIOGEARS_NAMESPACE SEPatient;

%ignore BIOGEARS_NAMESPACE SEPatient::Unload;
%ignore BIOGEARS_NAMESPACE SEPatient::Load;

%ignore BIOGEARS_NAMESPACE SEPatient::GetScalar(std::string const &);
%ignore BIOGEARS_NAMESPACE SEPatient::SetName(std::string const &);
%ignore BIOGEARS_NAMESPACE SEPatient::SetAnnotation(std::string const &);

//
//  Looks like SWIG can't handle trampoling CSharp code to C++  
//
%ignore BIOGEARS_NAMESPACE SEPatient::SetEventCallback;

%rename(GetBodyFatFraction_const) BIOGEARS_NAMESPACE SEPatient::GetBodyFatFraction() const;
%rename(GetHyperhidrosis_const) BIOGEARS_NAMESPACE SEPatient::GetHyperhidrosis() const;
%rename(GetPainSusceptibility_const) BIOGEARS_NAMESPACE SEPatient::GetPainSusceptibility() const;
%rename(GetRightLungRatio_const) BIOGEARS_NAMESPACE SEPatient::GetRightLungRatio() const;


%include <biogears/cdm/patient/SEPatient.h>
