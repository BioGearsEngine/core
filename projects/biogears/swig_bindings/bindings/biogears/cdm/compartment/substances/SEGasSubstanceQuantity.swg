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

%nspace BIOGEARS_NAMESPACE SEGasSubstanceQuantity;

%ignore BIOGEARS_NAMESPACE SEGasSubstanceQuantity::Unload;
%ignore BIOGEARS_NAMESPACE SEGasSubstanceQuantity::Load;

%ignore BIOGEARS_NAMESPACE SEGasSubstanceQuantity::GetScalar(const std::string&& name);

%ignore BIOGEARS_NAMESPACE SEGasSubstanceQuantity::GetScalar(std::string const &);
%rename (GetVolumeFraction_const) BIOGEARS_NAMESPACE SEGasSubstanceQuantity::GetVolumeFraction() const;


%warnfilter(401) BIOGEARS_NAMESPACE SEGasSubstanceQuantity;
%include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>

%nspace std::vector<BIOGEARS_NAMESPACE SEGasSubstanceQuantity*>;
%template (vector_SEGasSubstanceQuantity) std::vector<BIOGEARS_NAMESPACE SEGasSubstanceQuantity*>;