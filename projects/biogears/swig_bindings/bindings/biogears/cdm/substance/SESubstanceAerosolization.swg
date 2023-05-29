%{
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarNeg1To1.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEHistogramFractionVsLength.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>


%nspace BIOGEARS_NAMESPACE SESubstanceAerosolization;

%ignore BIOGEARS_NAMESPACE SESubstanceAerosolization::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceAerosolization::Load;

%ignore BIOGEARS_NAMESPACE SESubstanceAerosolization::operator==(SESubstanceAerosolization const& rhs) const;
%ignore BIOGEARS_NAMESPACE SESubstanceAerosolization::operator!=(SESubstanceAerosolization const& rhs) const;
%ignore BIOGEARS_NAMESPACE SESubstanceAerosolization::operator=(SESubstanceAerosolization const& rhs);

%ignore BIOGEARS_NAMESPACE SESubstanceAerosolization::GetScalar(std::string const &);
%rename (GetBronchioleModifier_const) BIOGEARS_NAMESPACE SESubstanceAerosolization::GetBronchioleModifier() const;
%rename (GetInflammationCoefficient_const) BIOGEARS_NAMESPACE SESubstanceAerosolization::GetInflammationCoefficient() const;
%rename (GetParticulateSizeDistribution_const) BIOGEARS_NAMESPACE SESubstanceAerosolization::GetParticulateSizeDistribution() const;

%include <biogears/cdm/substance/SESubstanceAerosolization.h>
