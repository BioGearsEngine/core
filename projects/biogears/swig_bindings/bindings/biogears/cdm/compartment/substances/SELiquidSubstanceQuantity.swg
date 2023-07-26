%{
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SELiquidSubstanceQuantity
#endif

%include <biogears/exports.swg>

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