%{
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
%}
%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/compartment/substances/SESubstanceQuantity.swg>
%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>

%nspace biogears::SELiquidSubstanceQuantity;

%ignore biogears::SELiquidSubstanceQuantity::Unload;
%ignore biogears::SELiquidSubstanceQuantity::Load;

%rename (GetSaturation_const) biogears::SELiquidSubstanceQuantity::GetSaturation() const; 
%ignore biogears::SELiquidSubstanceQuantity::GetScalar(std::string const &);

%warnfilter(401) biogears::SELiquidSubstanceQuantity;
%include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>

%nspace std::vector<biogears::SELiquidSubstanceQuantity*>;
%template (vector_SELiquidSubstanceQuantity) std::vector<biogears::SELiquidSubstanceQuantity*>;