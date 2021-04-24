%{
#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
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
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>

%nspace biogears::SEGasSubstanceQuantity;

%ignore biogears::SEGasSubstanceQuantity::Unload;
%ignore biogears::SEGasSubstanceQuantity::Load;

%ignore biogears::SEGasSubstanceQuantity::GetScalar(const std::string&& name);

%ignore biogears::SEGasSubstanceQuantity::GetScalar(std::string const &);
%rename (GetVolumeFraction_const) biogears::SEGasSubstanceQuantity::GetVolumeFraction() const;


%warnfilter(401) biogears::SEGasSubstanceQuantity;
%include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>

%nspace std::vector<biogears::SEGasSubstanceQuantity*>;
%template (vector_SEGasSubstanceQuantity) std::vector<biogears::SEGasSubstanceQuantity*>;