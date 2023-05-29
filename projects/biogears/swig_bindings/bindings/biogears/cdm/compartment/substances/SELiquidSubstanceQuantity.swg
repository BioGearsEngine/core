%{
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
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

%nspace BIOGEARS_NAMESPACE SELiquidSubstanceQuantity;

%ignore BIOGEARS_NAMESPACE SELiquidSubstanceQuantity::Unload;
%ignore BIOGEARS_NAMESPACE SELiquidSubstanceQuantity::Load;

%rename (GetSaturation_const) BIOGEARS_NAMESPACE SELiquidSubstanceQuantity::GetSaturation() const; 
%ignore BIOGEARS_NAMESPACE SELiquidSubstanceQuantity::GetScalar(std::string const &);

%warnfilter(401) BIOGEARS_NAMESPACE SELiquidSubstanceQuantity;
%include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>

%nspace std::vector<BIOGEARS_NAMESPACE SELiquidSubstanceQuantity*>;
%template (vector_SELiquidSubstanceQuantity) std::vector<BIOGEARS_NAMESPACE SELiquidSubstanceQuantity*>;