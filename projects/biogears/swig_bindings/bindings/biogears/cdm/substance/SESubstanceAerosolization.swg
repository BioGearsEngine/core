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


%nspace biogears::SESubstanceAerosolization;

%ignore biogears::SESubstanceAerosolization::Unload;
%ignore biogears::SESubstanceAerosolization::Load;

%ignore biogears::SESubstanceAerosolization::operator==(SESubstanceAerosolization const& rhs) const;
%ignore biogears::SESubstanceAerosolization::operator!=(SESubstanceAerosolization const& rhs) const;
%ignore biogears::SESubstanceAerosolization::operator=(SESubstanceAerosolization const& rhs);

%ignore biogears::SESubstanceAerosolization::GetScalar(std::string const &);
%rename (GetBronchioleModifier_const) biogears::SESubstanceAerosolization::GetBronchioleModifier() const;
%rename (GetInflammationCoefficient_const) biogears::SESubstanceAerosolization::GetInflammationCoefficient() const;
%rename (GetParticulateSizeDistribution_const) biogears::SESubstanceAerosolization::GetParticulateSizeDistribution() const;

%include <biogears/cdm/substance/SESubstanceAerosolization.h>
